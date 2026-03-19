#include "stdafx.h"
#include "PlayerPushState.h"

namespace
{
	const auto MOVE_SPEED = 1.0f /60.0f; //! 突き進む攻撃の移動速度。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerPushState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* キャスト。*/
			m_player->PlayWeaponAnimation(AttackType::PushForward);

			/* 武器の角度を調整。*/
			m_player->SetWeaponRotationAngle(Vector3::Front, -90.0);
		}


		void PlayerPushState::Update()
		{
			/* 親クラスの更新。*/
			PlayerAttackBaseState::Update();

			/* 前進する。*/
			MoveForward();
		}


		void PlayerPushState::Exit()
		{
			m_player->SetWeaponRotationAngle(Vector3::Right, 0.0f);
		}


		void PlayerPushState::MoveForward()
		{
			/* 前進する速度を設定。*/
			SetForwardSpeed(200.0f);

			/* 前方向のベクトルを取得。*/
			m_moveVector = m_player->GetForwardVector() * m_forwardSpeed;

			/* 当たり判定の移動。*/
			m_player->GetCharacterController().Execute(m_moveVector, MOVE_SPEED);
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());
		}


		bool PlayerPushState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enPushForward, id);
		}
	}
}