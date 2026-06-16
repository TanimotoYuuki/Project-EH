#include "stdafx.h"
#include "BossDethState.h"
#include "Boss.h"

#include "Src/Sound/SoundLister.h"
#include "Src/Sound/SEList.h"

namespace nsApp
{
	namespace nsState
	{
		void BossDethState::Enter()
		{
			m_boss = static_cast<nsActor::Boss *>(m_owner);

			if (m_boss)
			{
				m_boss->PlayAnimation(nsActor::BossAnimationID::Death);

				/*€–S‚Ì‰¹‚ğÄ¶B*/
				auto soundManager = FindGO<nsSound::SoundLister>("SoundManager");

				if (soundManager != nullptr && reinterpret_cast<uintptr_t>(soundManager))
				{
					soundManager->GetSEList().PlaySE(nsSound::SE_ID::Death, 1.0f, false, 100.0f);
				}
			}
		}

		void BossDethState::Update()
		{
		}

		void BossDethState::Exit()
		{
		}

		bool BossDethState::RequestID(uint8_t &id)
		{
			/*€‚ñ‚¾‚ç‘JˆÚ‚µ‚È‚¢B*/
			return false;
		}
	}
}