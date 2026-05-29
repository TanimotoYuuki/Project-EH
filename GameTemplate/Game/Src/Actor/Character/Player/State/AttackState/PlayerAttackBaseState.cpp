#include "stdafx.h"
#include "PlayerAttackBaseState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"

#include "Src/Actor/Character/Status/AttackParameterTable.h"
#include "Src/Actor/Magic/Factory/MagicFactory.h"
#include "Boss.h"

namespace
{
	const auto ATTACK_END_FRAME = 5;			//! 攻撃終了フレーム。
	const auto RUSH_COMBO_THRESHOLD = 2;		//! 連続攻撃の閾値。
	const auto HIT_STOP_FRAME = 8;              //! ヒットストップのフレーム数。
	const auto DAMAGE_TEXT_OFFSET_Y = 120.0f;   //! ダメージテキストのY軸オフセット。
	const auto CRITICAL_PERCENTAGE = 100.0f;    //! クリティカル発生確立。
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
			OnEnterAttack();
		}


		void PlayerAttackBaseState::Update()
		{
			/* タイマーを加算する。*/
			m_attackTimer++;

			/* 入力クラスを取得する。*/
			const auto& inputClass = m_player->GetInputClass();


			// @TODO: リファ。
/////////////////////////////////////////////////////////////////////////////////////////////////

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

			if (inputClass.IsPressX())
				m_inputRequests[ComboInputType::PressX] = true;

			/* Aボタンアクション。*/
			if (inputClass.IsSlashUp())
				m_inputRequests[ComboInputType::PressLB2] = true;
			else if (inputClass.IsJump())
				m_inputRequests[ComboInputType::PressA] = true;

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
				auto& hitDetection = m_player->GetWeaponHitDetection();

				if (!m_isHit && hitDetection.IsHit(m_boss))
				{
					m_isHit = true;
					ApplyDamageToText(m_boss);

					m_player->SetHitStop(HIT_STOP_FRAME);
					m_boss->SetHitStop(HIT_STOP_FRAME);
				}

				/* 終了判定。*/
				if (UseCommonEndTransition() && m_attackTimer > ATTACK_END_FRAME && !m_player->IsPlayAnimation())
				{
					m_stateMachine->ChangeState(new PlayerIdleState());
					return;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////
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
			/* 攻撃力を取得。*/
			const auto& playerStatus = m_player->GetCharacterStatus().attack;
			/* 現在の攻撃パラメーターの情報を取得する。*/
			const auto& attackParameter = AttackParameterTable::GetAttackParameter(m_currentAttackType);
			
			/* ダメージ計算。*/
		    auto finalDamage = static_cast<int>(playerStatus.normalDamage * attackParameter.damageMultiplier);
			const auto criticalRate = playerStatus.criticalRate + attackParameter.criticalRatel;

			/* クリティカル判定。*/
			if ((rand() % static_cast<int>(CRITICAL_PERCENTAGE)) < (criticalRate + CRITICAL_PERCENTAGE))
				finalDamage = static_cast<int>(finalDamage * playerStatus.criticalDamage);

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