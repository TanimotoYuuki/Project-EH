#include "stdafx.h"
#include "PlayerDethState.h"

namespace
{
	const auto MOVE_FRAME_RATE = 1.0f / 60.0f; //! 1フレームあたりの固定時間。
	const auto FALLINGSPEED = 30.0f;           //! 落下速度。
	const auto FALL_VELOCITY = 0.0f;           //! 落下速度。
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

			/* 死亡位置の保存。*/
			if (!m_hasDeathPosition)
			{
				/* 死亡位置を保存。*/
				m_dethPosition = m_player->GetCharacterController().GetPosition();
				m_hasDeathPosition = true;
			}

			/* 死亡位置に移動。*/
			m_player->SetPostureOffset(Quaternion::Identity);
			/* 死亡位置に移動。*/
			m_player->SetWeaponRotationByQuaternion(Quaternion::Identity);
			/* 落下速度を0に。*/
			m_player->SetFallVelocity(FALL_VELOCITY);
			/* 入力を無効化。*/
			m_player->SetInputEnable(false);
			/* 死亡位置に移動。*/
			m_player->SetPosition(m_dethPosition);
			/* 死亡位置に移動。*/
			m_player->GetCharacterController().SetPosition(m_dethPosition);

			/* アニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Death);
		}
	}
}