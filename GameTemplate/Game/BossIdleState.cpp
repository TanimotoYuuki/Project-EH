#include "stdafx.h"
#include "BossIdleState.h"
#include "Boss.h"
#include <random>


namespace
{
	const float IDLE_TIME = 2.0f;

	std::mt19937 g_randomGen(std::random_device{}());

	uint8_t DecideNextAction(
		nsApp::nsActor::Boss* boss,
		float distance,
		float hpRatio)
	{
		using namespace nsApp::nsActor;
		using namespace nsApp::nsAI;

		float roarThreshold1 = BossAIConfig::ROAR_HP_THRESHOLD_FIRST;
		float roarThreshold2 = BossAIConfig::ROAR_HP_THRESHOLD_SECOND;

		if (hpRatio <= roarThreshold1 && hpRatio > roarThreshold2 &&
			boss->GetRoarUsageCount() < 1 &&
			boss->CanUseRoar())
		{
			boss->UseRoar();
			return static_cast<uint8_t>(BossStateID::enRoar);
		}

		if (hpRatio <= roarThreshold2 &&
			boss->GetRoarUsageCount() < 2 &&
			boss->CanUseRoar())
		{
			boss->UseRoar();
			return static_cast<uint8_t>(BossStateID::enRoar);
		}

		if (distance < BossAIConfig::CLOSE_ATTACK_DISTANCE)
		{
			return static_cast<uint8_t>(BossStateID::enAttack);
		}
		else if (distance < BossAIConfig::MEDIUM_ATTACK_DISTANCE)
		{
			std::uniform_int_distribution<int> dist(0, 99);
			int choice = dist(g_randomGen);

			if (hpRatio > BossAIConfig::HIGH_HP_RATIO)
			{
				if (choice < 50)
				{
					return static_cast<uint8_t>(BossStateID::enMove);
				}
				else
				{
					return static_cast<uint8_t>(BossStateID::enAttack);
				}
			}
			else if (hpRatio < BossAIConfig::LOW_HP_RATIO)
			{
				if (choice < 70)
				{
					return static_cast<uint8_t>(BossStateID::enMove);
				}
				else
				{
					return static_cast<uint8_t>(BossStateID::enAttack);
				}
			}
			else
			{
				if (choice < 60)
				{
					return static_cast<uint8_t>(BossStateID::enMove);
				}
				else
				{
					return static_cast<uint8_t>(BossStateID::enAttack);
				}
			}
		}
		else
		{
			std::uniform_int_distribution<int> dist(0, 99);
			int choice = dist(g_randomGen);

			if (hpRatio > BossAIConfig::HIGH_HP_RATIO)
			{
				if (choice < BossAIConfig::ROAR_MOVE_CHANCE)
				{
					return static_cast<uint8_t>(BossStateID::enMove);
				}
				else
				{
					return static_cast<uint8_t>(BossStateID::enRoar);
				}
			}
			else
			{
				if (choice < BossAIConfig::ROAR_ATTACK_CHANCE)
				{
					return static_cast<uint8_t>(BossStateID::enMove);
				}
				else
				{
					return static_cast<uint8_t>(BossStateID::enRoar);
				}
			}
		}
	}
}

namespace nsApp
{
	namespace nsState
	{
		void BossIdleState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);

			m_timer = IDLE_TIME;

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
			if (m_timer <= 0.0f)
			{
				if (!m_boss || !m_boss->GetTarget() || m_boss->GetTarget()->IsDead())
				{
					m_timer = IDLE_TIME;
					return false;
				}

				float distance = m_boss->GetDistanceToTarget();
				float hpRatio = m_boss->GetHPRatio();

				id = DecideNextAction(m_boss, distance, hpRatio);

				return true;
			}
			return false;
		}
	}
}
