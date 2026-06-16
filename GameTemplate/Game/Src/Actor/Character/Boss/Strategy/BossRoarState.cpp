#include "stdafx.h"
#include "BossRoarState.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include <random>

namespace
{
	const float ROAR_DURATION = 5.0f;
	std::mt19937 g_randomGen(std::random_device{}());
}

namespace nsApp
{
	namespace nsState
	{
		void BossRoarState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);
			m_timer = ROAR_DURATION;
			m_boss->PlayAnimation(nsActor::BossAnimationID::Scream);
		}

		void BossRoarState::Update()
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}


		bool BossRoarState::RequestID(uint8_t& id)
		{
			if (m_timer <= 0.0f)
			{
				using namespace nsAI;

				std::uniform_int_distribution<int> dist(0, 99);
				int choice = dist(g_randomGen);

				float hpRatio = m_boss->GetHPRatio();

				if (hpRatio < BossAIConfig::LOW_HP_RATIO)
				{
					if (choice < 70)
					{
						id = static_cast<uint8_t>(nsActor::BossStateID::enMove);
					}
					else
					{
						id = static_cast<uint8_t>(nsActor::BossStateID::enAttack);
					}
				}
				else
				{
					if (choice < BossAIConfig::ROAR_MOVE_CHANCE)
					{
						id = static_cast<uint8_t>(nsActor::BossStateID::enMove);
					}
					else
					{
						id = static_cast<uint8_t>(nsActor::BossStateID::enAttack);
					}
				}
				return true;
			}
			return false;
		}
	}
}

