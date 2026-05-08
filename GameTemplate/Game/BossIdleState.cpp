#include "stdafx.h"
#include "BossIdleState.h"
#include "Boss.h"


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
				id = static_cast<uint8_t>(nsActor::BossStateID::enMove);
				return true;
			}
			return false;
		}
	}
}