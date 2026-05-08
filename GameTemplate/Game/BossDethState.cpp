#include "stdafx.h"
#include "BossDethState.h"
#include "Boss.h"

namespace nsApp
{
	namespace nsState
	{
		void BossDethState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);

			m_boss->PlayAnimation(nsActor::BossAnimationID::Death);
		}

		void BossDethState::Update()
		{

		}

		void BossDethState::Exit()
		{

		}

		bool BossDethState::RequestID(uint8_t& id)
		{
			/*éÄÇÒÇæÇÁâΩÇ‡ÇµÇ»Ç¢ÅB*/
			return false;
		}
	}
}