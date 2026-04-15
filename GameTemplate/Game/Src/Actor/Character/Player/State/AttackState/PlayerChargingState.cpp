#include "stdafx.h"
#include "PlayerChargingState.h"

namespace
{
	const auto EFFECT_SCALE = Vector3::One * 5.0f; /* エフェクトの大きさ。*/
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

			/* エフェクトを再生する。*/
			m_effectPosition = m_player->GetPosition();
			m_effectPosition.y += 50.0f;
			m_chargeEffect = m_player->GetEffectList().PlayEffect(nsEffect::Charge, m_effectPosition);

			if (m_chargeEffect != nullptr)
				m_chargeEffect->SetScale(EFFECT_SCALE);
			
			m_chargingTimer = 0;
		}


		void PlayerChargingState::Update()
		{
			/* チャージタイマーを加算する。*/
			m_chargingTimer++;

			if (m_chargingTimer == 10)
			{
				m_effectPosition = m_player->GetPosition();
				m_effectPosition.y += 50.0f;
				m_chargeEffect = m_player->GetEffectList().PlayEffect(nsEffect::Charge, m_effectPosition);

				if (m_chargeEffect != nullptr)
					m_chargeEffect->SetScale(EFFECT_SCALE);
			}

			/* エフェクトが生成されたあとの追従処理 */
			if (m_chargeEffect != nullptr)
			{
				m_chargeEffect->Update();
				m_chargeEffect->SetScale(EFFECT_SCALE);
				Vector3 currentEffectPos = m_player->GetPosition();
				currentEffectPos.y = 50.0f; 
				m_chargeEffect->SetPosition(currentEffectPos);
			}
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
