#include "stdafx.h"
#include "PlayerAttackBaseState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"

#include "Src/Actor/Character/Status/AttackParameterTable.h"
#include "Src/Actor/Magic/Factory/MagicFactory.h"
#include "Src/Actor/Character/Boss/Boss.h"

#include <algorithm>
#include <cstdlib>


namespace
{
	const int CRITICAL_PERCENTAGE = 100;		//! クリティカル率の計算に使用する定数。
	const auto ATTACK_END_FRAME = 5;			//! 攻撃終了フレーム。
	const auto DAMAGE_TEXT_OFFSET_Y = 120.0f;   //! ダメージテキストのY軸オフセット。

	/**
	 * @brief 値を指定範囲内に収める。
	 * @param value 値。
	 * @param minValue 最小値。
	 * @param maxValue 最大値。
	 * @return 範囲内に収めた値。
	 */
	float ClampFloat(float value, float minValue, float maxValue)
	{
		/* 最小値を補正。*/
		if (value < minValue)
			return minValue;

		/* 最大値を補正。*/
		if (value > maxValue)
			return maxValue;

		/* 補正値を返す。*/
		return value;
	}
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerAttackBaseState::Enter()
		{
			/* 攻撃の種類ごとにキャストを行う。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* ボスクラスを探索。*/
			m_boss = FindGO<nsActor::Boss>("boss");

			/* 共通の初期化。*/
			OnCommonInitializeToEnter();

			/* アニメーションの再生と固有の初期化は派生クラスに譲渡。*/
			PlayAttackAnimation();

			/* 攻撃内容を実況UIへ通知する。*/
			NotifyAttackCommentary();

			/* 子クラスの初期化。*/
			OnEnterAttack();
		}


		void PlayerAttackBaseState::Update()
		{
			/* タイマーを加算する。*/
			m_attackTimer++;

			/* 入力クラスを取得する。*/
			const auto& inputClass = m_player->GetInputClass();

			/* Bボタンアクション。*/
			if (inputClass.IsAttack())
			{
				/*
				 * タイマーを加算する。
				 * Bボタンを押すごとにタイマーを加算し、当てはまるなら連続攻撃に繋げる。
				 */
				m_rushCount++;

				/* Bボタンが押されていたら予約を入れる。*/
				m_inputRequests[ComboInputType::PressB] = true;
			}

			/* Xボタンアクション。*/
			if (inputClass.IsPressX())
				m_inputRequests[ComboInputType::PressX] = true;

			/* Aボタンアクション。*/
			if (inputClass.IsSlashUp())
				m_inputRequests[ComboInputType::PressLB2] = true;
			else if (inputClass.IsJump())
				m_inputRequests[ComboInputType::PressA] = true;

			/* 連続攻撃の条件を満たしていたら予約を入れる。*/
			if (m_rushCount >= 2)
				m_inputRequests[ComboInputType::RushB] = true;

			/* 毎フレームの処理とタイミングがある処理を派生クラスに譲渡。*/
			if (OnUpdateAttack())
				return;

			OnAttackTick();

			/* 終了判定。*/
			if (UseCommonEndTransition() && m_attackTimer > ATTACK_END_FRAME && !m_player->IsPlayAnimation())
			{
				/* Idle状態へ遷移。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
				return;
			}

			/* 衝突判定。*/
			if (m_boss != nullptr)
			{
				/* プレイヤーの攻撃当たり判定を取得。*/
				auto& hitDetection = m_player->GetWeaponHitDetection();

				if (!m_isHit && hitDetection.IsHit(m_boss))
				{
					/* ヒットフラグをセット。*/
					m_isHit = true;

					/* 攻撃パラメータを取得。*/
					const auto& attackParameter = GetCurrentAttackParameter();

					/* ダメージテキストの表示とダメージの計算を行う処理。*/
					ApplyDamageToText(m_boss);

					/* ヒットストップする機能をセット。*/
					m_player->SetHitStop(attackParameter.hitStopFrame);
					m_boss->SetHitStop(attackParameter.hitStopFrame);
				}

				/* 終了判定。*/
				if (UseCommonEndTransition() && m_attackTimer > ATTACK_END_FRAME && !m_player->IsPlayAnimation())
				{
					m_stateMachine->ChangeState(new PlayerIdleState());
					return;
				}
			}
		}


		void PlayerAttackBaseState::Exit()
		{
			/* Stateを抜ける際の処理。*/
			/* 効果音とかエフェクトとかの再生をストップさせる*/
			if (m_player)
			{
				/* 当たり判定を奪取する。*/
				m_player->GetWeaponHitDetection().Disable();

				/* SEの再生を止める。*/
				m_player->StopWeaponSE();
			}

			/* 子クラスの終了処理。*/
			OnExitAttack();
		}


		void PlayerAttackBaseState::NotifyAttackCommentary()
		{
			/* Playerクラスが存在するか検知。*/
			if (m_player == nullptr)
				return;

			/* 実況用の攻撃名を取得する。*/
			const auto actionName = GetCommentaryActionName();
			if (actionName.empty())
				return;

			/* CommentaryUIManagerクラスを探索。*/
			auto* commentary = FindGO<nsUI::CommentaryUIManager>("CommentaryUIManager");
			if (commentary == nullptr)
				return;

			/* 攻撃内容を実況UIへ通知する。*/
			commentary->AddActionMessage(m_player->GetCurrentWeapon(), actionName);
		}


		std::wstring PlayerAttackBaseState::GetCommentaryActionName() const
		{
			switch (m_currentAttackType)
			{
			case AttackType::NormalAttack:
				return L"こうげき！";

			case AttackType::HeavyAttack:
				return L"いちげき！";

			case AttackType::ChargeAttack:
				return L"ためこうげき！";

			case AttackType::HeelMagic:
				return L"かいふく！";

			case AttackType::MagicAttack:
				return L"まほう！";

			case AttackType::AirAttack:
				return L"くうちゅう！";

			case AttackType::RushAttack_Start:
				return L"れんぞく！";

			case AttackType::RushAttack_End:
				return L"フィニッシュ！";

			case AttackType::SlashUp:
				return L"コンボ！";

			case AttackType::PushForward:
				return L"とっしん！";

			default:
				return L"";
			}
		}


		bool PlayerAttackBaseState::CheckCombo(PLAYER_STATE_ID currentStateID, uint8_t& id)
		{
			/* playerクラスが存在するか検知。*/
			if (!m_player)
				return false;

			/* 地上にいるかどうかを確認。*/
			m_isGrounded = m_player->GetCharacterController().IsOnGround();

			/* ステートIDと地上にいるかどうかを検知させる。*/
			const auto& routes = ComboRouteTable::GetRoutes(currentStateID, m_isGrounded);

			for (const auto& route : routes)
			{
				/* テーブルから指定されたものを取り出す。*/
				m_isInputMatch = m_inputRequests[route.inputType];

				/* 時間と入力条件を満たしているか確認。*/
				if (m_attackTimer >= route.cancelTime && m_isInputMatch)
				{
					/* 条件を満たしている場合、次の状態へ遷移する。*/
					id = static_cast<uint8_t>(route.nextStateID);
					return true;
				}
			}
			return false;
		}


		void PlayerAttackBaseState::ApplyDamageToText(nsActor::ICharacter* target)
		{
			/* playerクラスとtargetが存在するか検知。*/
			if (!m_player || !target)
				return;

			/* 最終的なダメージ。*/
			const int damageAmount = CalculateFinalDamage();

			/* ダメージテキストの表示位置。*/
			m_damageRequest = BuildDamageRequest(target, damageAmount);

			/* ダメージテキストを表示する。*/
			DamageProcessor::ApplyDamage(m_damageRequest);
		}


		int PlayerAttackBaseState::CalculateFinalDamage() const
		{
			/* Playerクラスがいるか検知。*/
			if (m_player == nullptr)
				return 0;

			/* 攻撃力を取得。*/
			const auto& playerStatus = m_player->GetCharacterStatus().attack;

			/* 攻撃パラメータを取得。*/
			const auto& attackParameter = GetCurrentAttackParameter();

			/* ダメージ数を計算。元の処理と同じく、基礎ダメージ × 攻撃倍率で計算する。*/
			int finalDamage = static_cast<int>(playerStatus.normalDamage * attackParameter.damageMultiplier);

			/* クリティカル率を計算。*/
			float criticalRate = playerStatus.criticalRate + attackParameter.criticalRatel;
			criticalRate = ClampFloat(criticalRate, 0.0f, 1.0f);

			/* クリティカルの閾値を計算。*/
			const int criticalThreshold = static_cast<int>( criticalRate * static_cast<float>(CRITICAL_PERCENTAGE));

			/* クリティカル判定。*/
			if ((rand() % CRITICAL_PERCENTAGE) < criticalThreshold)
				finalDamage = static_cast<int>(finalDamage * playerStatus.criticalDamage);

			/* 最終的なダメージ数を計算。*/
			finalDamage = static_cast<int>(finalDamage * m_player->GetAttackDamageRate());

			/* ダメージが0以下で、かつダメージ倍率が0より大きい場合は1にする。*/
			if (finalDamage <= 0 && attackParameter.damageMultiplier > 0.0f)
				finalDamage = 1;

			return finalDamage;
		}


		DamageRequest PlayerAttackBaseState::BuildDamageRequest(nsActor::ICharacter* target, int damageAmount) const
		{
			DamageRequest request;
			request.target = target;
			request.damageAmount = damageAmount;
			request.hitPosition = target->GetPosition();
			request.hitPosition.y += DAMAGE_TEXT_OFFSET_Y;

			return request;
		}
	}
}