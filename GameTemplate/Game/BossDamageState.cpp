#include "stdafx.h"
#include "BossDamageState.h"
#include "Boss.h"

namespace nsApp
{
	namespace nsState
	{
		void BossDamageState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);

			m_timer = 0.5f;

			m_boss->PlayAnimation(nsActor::BossAnimationID::GetHit);
		}

		void BossDamageState::Update()
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}

		void BossDamageState::Exit()
		{

		}

		bool BossDamageState::RequestID(uint8_t& id)
		{
			if (m_timer <= 0.0f)
			{
				id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
				return true;
			}
			return false;
		}
	}
}
