#include "stdafx.h"
#include "PlayerChargingState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/Component/ComboRouteTable.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"

#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerMagicAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerHeelMagicState.h"

namespace
{
	const auto EFFECT_SCALE = Vector3::One * 5.0f;	//! エフェクトの大きさ。
	const auto EFFECT_POSITION = 50.0f;				//! エフェクトの位置。
	const auto FIRE_EFFECT_ANGLE = 22.5f;			//! 炎エフェクトの角度。
	const auto FIRE_EFFECT_SCALE_BASE = 1.25f;		//! 炎エフェクトの基本の大きさ。
	const auto FIRE_EFFECT_SCALE_MULTIPLIER = 2.5f; //! 炎エフェクトの大きさの倍率。
	const auto CHARGE_TIMER = 10;					//! チャージエフェクトを生成するためのフレーム数。

	constexpr int kRequiredChargeFrame = 30; //! チャージ攻撃が発動するために必要なフレーム数。
	constexpr int MAX_CHARGING_FRAMES = 120; //! これ以上チャージが続くと強制的にチャージ攻撃を行う。

	constexpr int EFFECTRECREATION_INTERVAL = 240; //! エフェクトの再生成間隔。
	constexpr float EFFECT_LIFE_TIME = 999999.0f;  //! エフェクトの寿命。
	constexpr float MAX_CHARGING_SECONDS = 3.5f;   //! チャージが続く最大時間（秒）。
}

/** @def
 *  武器の当たり判定を取得するためのマクロ。
 */
#define HIT_DETECTION m_player->GetWeaponHitDetection()

namespace nsApp
{
	namespace nsState
	{
		void PlayerChargingState::Enter()
		{
			/*キャスト。*/
			m_player = static_cast<nsActor::Player *>(m_owner);

			/* アニメーションを再生する。*/
			m_player->PlayWeaponAnimation(AttackType::Charging);

			/* チャージタイマーを初期化する。*/
			m_chargingTimer = 0;
			m_chargingElapsedTime = 0.0f;
		}

		void PlayerChargingState::Update()
		{
			/* チャージタイマーを加算する。*/
			m_chargingTimer++;

			m_chargingElapsedTime += g_gameTime->GetFrameDeltaTime();

			/* チャージ段階を計算する。*/
			ComputeEffectLevel();

			/* チャージエフェクトを生成。*/
			CreateChargeEffect();

			/* ハンマーの炎エフェクトを生成。*/
			CreateFireEffect();

			/*異常にチャージが長く続いた場合は強制的にチャージ攻撃に遷移させる。*/
			if (m_chargingElapsedTime >= MAX_CHARGING_SECONDS)
			{
				m_stateMachine->ChangeState(new PlayerChargeAttackState());
				return;
			}

			if (m_player->GetInputClass().IsChargeAttack())
			{
				m_stateMachine->ChangeState(new PlayerChargeAttackState());
				return;
			}

			////////////////////////////////////////////////////////////////////////
			// リファ。
			/* ボタンアクションで派生。*/
			if (m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				constexpr int kRequiredChargeFrame = 30;
				const bool isChargeReady = m_chargingTimer >= kRequiredChargeFrame;

				/* チャージが足りない間はRB/RTを受け付けない。*/
				if (!isChargeReady)
					return;

				/* RBButtonで魔法攻撃状態に遷移。*/
				if (m_player->GetInputClass().IsPressRB())
				{
					m_stateMachine->ChangeState(new PlayerMagicAttackState());
					return;
				}

				/* RTボタンで回復魔法状態に遷移。*/
				if (m_player->GetInputClass().IsPressRT())
				{
					m_stateMachine->ChangeState(new PlayerHeelMagicState());
					return;
				}
			}
			////////////////////////////////////////////////////////////////////////
		}

		void PlayerChargingState::Exit()
		{
			/* エフェクトを停止する。*/
			if (m_chargeEffect != nullptr)
			{
				m_chargeEffect->Stop();
				m_player->GetEffectList().StopEffect(m_chargeEffect);
				m_chargeEffect = nullptr;
			}

			/* ハンマーの炎エフェクトを停止する。*/
			if (m_hammerEffect != nullptr)
			{
				m_hammerEffect->Stop();
				m_player->GetEffectList().StopEffect(m_hammerEffect);
				m_hammerEffect = nullptr;
			}

			/* 武器のSEを停止する。*/
			if (m_player)
				m_player->StopWeaponSE();
		}

		void PlayerChargingState::CreateChargeEffect()
		{
			const bool isFirstCreate = (m_chargingTimer == CHARGE_TIMER);
			const bool isRecreateTiming = (m_chargingTimer > CHARGE_TIMER) && ((m_chargingTimer - CHARGE_TIMER) % EFFECTRECREATION_INTERVAL == 0);

			if (isFirstCreate || isRecreateTiming)
			{
				/*古いエフェクトが残っていれば先に止める。*/
				if (m_chargeEffect != nullptr)
				{
					m_chargeEffect->Stop();
					m_player->GetEffectList().StopEffect(m_chargeEffect);
					m_chargeEffect = nullptr;
				}
				m_chargeEffect = m_player->GetEffectList().PlayEffect(nsEffect::Charge, GetChargeEffectPosition(), Quaternion::Identity, Vector3::One, EFFECT_LIFE_TIME);
			}

			/*未精製の場合、処理をスキップ。*/
			if (m_chargeEffect == nullptr)
			{
				return;
			}

			/* 大きさをセット。*/
			m_chargeEffect->SetScale(Vector3::One * GetChargeEffectScale());

			/* 座標をセット。*/
			m_chargeEffect->SetPosition(GetChargeEffectPosition());
		}

		void PlayerChargingState::CreateFireEffect()
		{
			const bool isFirstCreate = (m_chargingTimer == CHARGE_TIMER);
			const bool isRecreateTiming = (m_chargingTimer > CHARGE_TIMER) && ((m_chargingTimer - CHARGE_TIMER) % EFFECTRECREATION_INTERVAL == 0);

			if ((isFirstCreate || isRecreateTiming) && m_player->GetCurrentWeapon() == WeaponType::Hammer)
			{
				if (m_hammerEffect != nullptr)
				{
					m_hammerEffect->Stop();
					m_player->GetEffectList().StopEffect(m_hammerEffect);
					m_hammerEffect = nullptr;
				}
				m_hammerEffect = m_player->GetEffectList().PlayEffect(nsEffect::Fire, HIT_DETECTION.GetPosition(), Quaternion::Identity, Vector3::One, EFFECT_LIFE_TIME);
			}

			/* HammerCharacterクラスではない場合は処理をスキップする。*/
			if (m_hammerEffect == nullptr)
				return;

			/* 武器の情報を取得してエフェクトを生成する。*/
			m_weaponPosition = HIT_DETECTION.GetPosition();

			/* チャージレベルにあわせてエフェクトを大きくする。*/
			m_fireEffectScale = FIRE_EFFECT_SCALE_BASE + (m_currentEffectLevel * FIRE_EFFECT_SCALE_MULTIPLIER);
			m_hammerEffect->SetScale(Vector3::One * m_fireEffectScale);

			/* 角度を設定。*/
			m_fireEffectAngle.SetRotationDegZ(FIRE_EFFECT_ANGLE);
			m_hammerEffect->SetRotation(m_fireEffectAngle);

			/* 座標を設定。*/
			m_hammerEffect->SetPosition(m_weaponPosition);
		}
	}
}
