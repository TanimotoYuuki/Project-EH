#include "stdafx.h"
#include "BossIdleState.h"
#include "Boss.h"
#include <random>


namespace
{
	const float IDLE_TIME = 2.0f;/*待機時間。*/
}

namespace nsApp
{
	namespace nsState
	{
		void BossIdleState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);

			m_timer = IDLE_TIME;

			/*待機アニメーション。*/
			m_boss->PlayAnimation(nsActor::BossAnimationID::Idle);
		}

		void BossIdleState::Update()
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}

		void BossIdleState::Exit()
		{

		}

		bool BossIdleState::RequestID(uint8_t& id)
		{
			/*一定時間後にMoveへ移行。*/
			if (m_timer <= 0.0f)
			{
				static std::mt19937 gen(std::random_device{}());
				std::uniform_int_distribution<int> dist(0, 1);
				if (dist(gen) == 0)
				{
					id = static_cast<uint8_t>(nsActor::BossStateID::enMove);
				}
				else
				{
					id = static_cast<uint8_t>(nsActor::BossStateID::enRoar);
				}
				return true;
			}
			return false;
		}
	}
}