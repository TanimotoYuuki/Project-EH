#include "stdafx.h"
#include "PlayerDethState.h"

namespace
{
	const auto MOVE_FRAME_RATE = 1.0f / 60.0f; //! 1フレームあたりの固定時間。
	const auto FALLINGSPEED = 30.0f;           //! 落下速度。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerDethState::Enter()
		{
			/* キャスト。*/ 
			m_player = static_cast<nsActor::Player*>(m_owner);
			if (m_player == nullptr)
				return;

			if (!m_hasDeathPosition)
			{
				m_dethPosition = m_player->GetCharacterController().GetPosition();
				m_hasDeathPosition = true;
			}

			m_player->SetPostureOffset(Quaternion::Identity);
			m_player->SetWeaponRotationByQuaternion(Quaternion::Identity);
			m_player->SetFallVelocity(0.0f);
			m_player->SetInputEnable(false);

			m_player->SetPosition(m_dethPosition);
			m_player->GetCharacterController().SetPosition(m_dethPosition);

			/* アニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Death);
		}


		void PlayerDethState::Update()
		{
			
		}


		bool PlayerDethState::RequestID(uint8_t& id)
		{
			return false;
		}
	}
}