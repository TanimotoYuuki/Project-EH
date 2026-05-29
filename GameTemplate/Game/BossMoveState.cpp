#include "stdafx.h"
#include "BossMoveState.h"

namespace
{
	const float MOVE_TIME = 3.0f;/*移動時間。*/
	const float MOVE_SPEED = 10.0f;/*移動速度。*/
	const float STOP_DISTANCE = 50.0f;/*これ以上近づいたら止まる距離。*/
}

namespace nsApp
{
	namespace nsState
	{
		void BossMoveState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);

			m_timer = MOVE_TIME;

			/*歩きアニメーション。*/
			m_boss->PlayAnimation(nsActor::BossAnimationID::Walk);
		}

		void BossMoveState::Update()
		{
			/*時間減少。*/
			float dt = g_gameTime->GetFrameDeltaTime();
			m_timer -= dt;

			Vector3 moveDirection = Vector3::Zero;
			if (m_boss->GetTarget())
			{
				moveDirection = m_boss->GetTarget()->GetPosition() - m_boss->GetPosition();
				moveDirection.y = 0.0f;
				if (moveDirection.LengthSq() > FLT_EPSILON)
				{
					moveDirection.Normalize();
				}
			}

			if (moveDirection.LengthSq() > FLT_EPSILON)
			{
				m_boss->SetForward(moveDirection);
			}

			Vector3 moveStep = moveDirection * MOVE_SPEED;
			moveStep.z = 0.0f;

			m_boss->GetController().Execute(moveStep, dt);
		}


		void BossMoveState::Exit()
		{

		}

		bool BossMoveState::RequestID(uint8_t& id)
		{
			/*ターゲットへ近づいたら攻撃。*/
			if (m_boss->GetDistanceToTarget() < STOP_DISTANCE)
			{
				id = static_cast<uint8_t>(nsActor::BossStateID::enAttack);
				return true;
			}


			/*時間経過でIdleに戻す。*/
			if (m_timer <= 0.0f)
			{
				id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
				return true;
			}

			return false;
		}
	}
}