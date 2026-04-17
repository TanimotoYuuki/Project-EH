#include "stdafx.h"
#include "PlayerChargingState.h"
#include "Src/Actor/Character/Player/Component/ComboRouteTable.h"

namespace
{
	const auto EFFECT_SCALE = Vector3::One * 5.0f; //! エフェクトの大きさ。
	const auto EFFECT_POSITION = 50.0f;            //! エフェクトの位置。
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
			ComputeEffectLevelByPosition();

			/* 10フレーム目に1度だけエフェクトを生成する。*/
			if (m_chargingTimer == 10)
			{
				/* チャージエフェクトを生成する。*/
				CreateChargeEffect();

				/* HammerCharacterクラスにのみエフェクトの生成を適応する。*/
				if (m_player->GetCurrentWeapon() == WeaponType::Hammer)
					m_hammerEffect = m_player->GetEffectList().PlayEffect(nsEffect::Fire, HIT_DETECTION.GetPosition());
			}

			/* エフェクトが生成さえていないなら早期リターン。*/
			if (m_chargeEffect == nullptr)
				return;

			/* エフェクトの更新。*/
			m_chargeEffect->Update();

			/* チャージ段階に応じたエフェクトの大きさを求める。*/
			ComputeEffectLevelByScale();

			/* エフェクトの座標を追従更新する。*/
			UpdateEffectPosition();			

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


		void PlayerChargingState::CreateFireEffect()
		{
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
			m_fireEffectAngle.SetRotationDegZ(22.5f);
			m_hammerEffect->SetRotation(m_fireEffectAngle);

			/* 座標を設定。*/
			m_hammerEffect->SetPosition(m_weaponPosition);
		}
	}
}
