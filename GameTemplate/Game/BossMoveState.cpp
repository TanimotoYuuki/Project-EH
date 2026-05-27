#include "stdafx.h"
#include "BossMoveState.h"
#include "BossAIConfig.h"
#include "BossTypeManager.h"

namespace
{
	const float MOVE_TIME = 3.0f;
	const float MOVE_SPEED = 10.0f;
}

namespace nsApp
{
	namespace nsState
	{
		void BossMoveState::Enter()
		{
			m_boss = static_cast<nsActor::Boss *>(m_owner);

			m_timer = MOVE_TIME;

			m_boss->PlayAnimation(nsActor::BossAnimationID::Walk);
		}

		void BossMoveState::Update()
		{
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

			/*ボスタイプに応じた移動速度を取得。*/
			const auto &params = nsApp::nsAI::BossTypeManager::GetBossTypeParameters(m_boss->GetBossType());
			Vector3 moveStep = moveDirection * params.m_moveSpeed;
			moveStep.z = 0.0f;

			m_boss->GetController().Execute(moveStep, dt);
		}

		void BossMoveState::Exit()
		{
		}

		bool BossMoveState::RequestID(uint8_t &id)
		{
			/*ボスタイプに応じた移動停止距離を取得。*/
			const auto &params = nsApp::nsAI::BossTypeManager::GetBossTypeParameters(m_boss->GetBossType());
			if (m_boss->GetDistanceToTarget() < params.m_moveStopDistance)
			{
				id = static_cast<uint8_t>(nsActor::BossStateID::enAttack);
				return true;
			}

			if (m_timer <= 0.0f)
			{
				id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
				return true;
			}

			return false;
		}
	}
}