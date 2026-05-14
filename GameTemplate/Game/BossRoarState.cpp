#include "stdafx.h"
#include "BossRoarState.h"
#include "Boss.h"

namespace nsApp
{
	namespace nsState
	{
		void BossRoarState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);
			m_timer = 1.5f;
			m_boss->PlayAnimation(nsActor::BossAnimationID::Scream);
		}

		void BossRoarState::Update()
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}

		void BossRoarState::Exit()
		{
		}

		bool BossRoarState::RequestID(uint8_t& id)
		{
			if (m_timer <= 0.0f)
			{
				id = static_cast<uint8_t>(nsActor::BossStateID::enMove);
				return true;
			}
			return false;
		}
	}
}
