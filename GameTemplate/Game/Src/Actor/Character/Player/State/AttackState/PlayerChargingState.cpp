#include "stdafx.h"
#include "PlayerChargingState.h"
#include "Src/Actor/Character/Player/Component/ComboRouteTable.h"

namespace
{
	const auto EFFECT_SCALE = Vector3::One * 5.0f; //! エフェクトの大きさ。
	const auto EFFECT_POSITION = 50.0f;            //! エフェクトの位置。
	const auto FIRE_EFFECT_ANGLE = 22.5f;          //! 炎エフェクトの角度。
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
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションを再生する。*/
			m_player->PlayWeaponAnimation(AttackType::Charging);

			/* チャージタイマーを初期化する。*/
			m_chargingTimer = 0;
		}


		void PlayerChargingState::Update()
		{
			/* チャージタイマーを加算する。*/
			m_chargingTimer++;

			/* チャージ段階を計算する。*/
			ComputeEffectLevel();

			/* チャージエフェクトを生成。*/
			CreateChargeEffect();

			/* ハンマーの炎エフェクトを生成。*/
			CreateFireEffect();
		}


		void PlayerChargingState::Exit()
		{
			if (m_chargeEffect != nullptr)
			{
				m_chargeEffect->Stop();
				m_chargeEffect = nullptr;
			}

			if (m_hammerEffect != nullptr)
			{
				m_hammerEffect->Stop();
				m_hammerEffect = nullptr;
			}

			if (m_player)
				m_player->StopWeaponSE();
		}


		bool PlayerChargingState::RequestID(uint8_t& id)
		{
			const auto& inputClass = m_player->GetInputClass();

			/* Bボタンを離した瞬間、タイマーの時間を元に派生させる */
			if (inputClass.IsChargeAttack())
			{
				id = static_cast<uint8_t>(PLAYER_STATE_ID::enChargeAttack);
				return true;
			}
			else if (inputClass.IsNormalAttack())
			{
				id = static_cast<uint8_t>(PLAYER_STATE_ID::enNormalAttack);
				return true;
			}

			return false;
		}


		void PlayerChargingState::CreateChargeEffect()
		{
			/* 特定のフレーム数に1度だけ生成する。*/
			if (m_chargingTimer == 10)
				m_chargeEffect = m_player->GetEffectList().PlayEffect(nsEffect::Charge, GetChargeEffectPosition());

			/* 未生成の場合、処理をスキップ。*/
			if (m_chargeEffect == nullptr)
				return;

			/* 更新。*/
			m_chargeEffect->Update();

			/* 大きさをセット。*/
			m_chargeEffect->SetScale(Vector3::One * GetChargeEffectScale());

			/* 座標をセット。*/
			m_chargeEffect->SetPosition(GetChargeEffectPosition());
		}


		void PlayerChargingState::CreateFireEffect()
		{
			if (m_chargingTimer == 10)
			{
				if(m_player->GetCurrentWeapon() == WeaponType::Hammer)
					m_hammerEffect = m_player->GetEffectList().PlayEffect(nsEffect::Fire, HIT_DETECTION.GetPosition());
			}

			/* HammerCharacterクラスではない場合は処理をスキップする。*/
			if (m_hammerEffect == nullptr)
				return;

			/* エフェクトを更新する。*/
			m_hammerEffect->Update();

			/* 武器の情報を取得してエフェクトを生成する。*/
			m_weaponPosition = HIT_DETECTION.GetPosition();

			/* チャージレベルにあわせてエフェクトを大きくする。*/
			m_fireEffectScale = 1.25f + (m_currentEffectLevel * 2.5f);
			m_hammerEffect->SetScale(Vector3::One * m_fireEffectScale);

			/* 角度を設定。*/
			m_fireEffectAngle.SetRotationDegZ(FIRE_EFFECT_ANGLE);
			m_hammerEffect->SetRotation(m_fireEffectAngle);

			/* 座標を設定。*/
			m_hammerEffect->SetPosition(m_weaponPosition);
		}
	}
}
