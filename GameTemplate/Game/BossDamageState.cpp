#include "stdafx.h"
#include "BossDamageState.h"
#include "Boss.h"

#include "Src/Sound/SoundLister.h"
#include "Src/Sound/SEList.h"

namespace nsApp
{
	namespace nsState
	{
		void BossDamageState::Enter()
		{
			m_boss = static_cast<nsActor::Boss *>(m_owner);

			m_timer = 0.5f;

			/*被弾アニメーション再生。*/
			m_boss->PlayAnimation(nsActor::BossAnimationID::GetHit);
			// ダメージ判定をリセット
			m_boss->ResetPrevHP();

			/*被弾時の音再生。*/
			auto soundManager = FindGO<nsSound::SoundLister>("SoundManager");
			if (soundManager != nullptr && reinterpret_cast<uintptr_t>(soundManager))
			{
				soundManager->GetSEList().PlaySE(nsSound::SE_ID::HitDamage, 1.0f, false, 100.0f);
			}
		}

		void BossDamageState::Update()
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}

		void BossDamageState::Exit()
		{
		}

		bool BossDamageState::RequestID(uint8_t &id)
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
