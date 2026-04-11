#include "stdafx.h"
#include "PlayerDethState.h"

namespace
{
	const auto MOVE_FRAME_RATE = 1.0f / 60.0f; /* 1フレームあたりの固定時間。*/
	const auto FALLINGSPEED = 30.0f;           /* 落下速度。*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerDethState::Enter()
		{
			/* キャスト。*/ 
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Death);
		}


		void PlayerDethState::Update()
		{
			/* 空中で死亡した場合は、地面に落とす。*/
			if (!m_player->GetCharacterController().IsOnGround())
			{
				m_fallSpeed = Vector3::Zero;
				m_fallSpeed.y = -FALLINGSPEED; /* 落下速度。*/

				/* キャラコンを使って落下させる。*/
				m_player->GetCharacterController().Execute(m_fallSpeed, MOVE_FRAME_RATE);
				m_player->SetPosition(m_player->GetCharacterController().GetPosition());
			}
		}


		bool PlayerDethState::RequestID(uint8_t& id)
		{
			return false;
		}
	}
}