#include "stdafx.h"
#include "PlayerChargingState.h"

namespace
{
	const auto EFFECT_SCALE = Vector3::One * 5.0f; //! エフェクトの大きさ。
	const auto EFFECT_POSITION = 50.0f;            //! エフェクトの位置。
}

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
			
			m_chargeEffect = nullptr;
			m_chargingTimer = 0;
		}


		void PlayerChargingState::Update()
		{
			/* チャージタイマーを加算する。*/
			m_chargingTimer++;

			/* チャージ段階を計算する。*/
			ComputeEffectLevelByPosition();

			/* 10フレーム目に1度だけエフェクトを生成する。*/
			if(m_chargingTimer == 10)
			   /* チャージエフェクトを生成する。*/
			   CreateChargeEffect();

			/* エフェクトが生成さえていないなら早期リターン。*/
			if (m_chargeEffect == nullptr)
				return;

			/* エフェクトの更新。*/
			m_chargeEffect->Update();

			/* チャージ段階に応じたエフェクトの大きさを求める。*/
			ComputeEffectLevelByScale();

			/* エフェクトの座標を追従更新する。*/
			UpdateEffectPosition();			
		}


		void PlayerChargingState::Exit()
		{
			if (m_chargeEffect != nullptr)
			{
				m_chargeEffect->Stop();
				m_chargeEffect = nullptr;
			}
		}


		bool PlayerChargingState::RequestID(uint8_t& id)
		{
			const auto& inputClass = m_player->GetInputClass();

			/* Bボタンを離した瞬間、タイマーの時間を元に派生させる */
			if (inputClass.IsChargeAttack())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enChargeAttack);
				return true;
			}
			else if (inputClass.IsNormalAttack())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enNormalAttack);
				return true;
			}

			return false;
		}
	}
}
