#include "stdafx.h"
#include "BiteAttackStrategy.h"
#include "Boss.h"

#include "Src/Sound/SoundLister.h"
#include "Src/Sound/SEList.h"

namespace nsApp
{
	namespace nsAI
	{
		void BiteAttackStrategy::Enter(nsActor::Boss *boss)
		{
			/*攻撃時間。*/
			m_timer = 1.0f;

			/*攻撃フラグ初期化。*/
			m_isAttack = false;

			/*SEのフラグを初期化。*/
			m_hasPlayedSound = false;

			/*Y位置をロック。*/
			boss->LockYPosition(boss->GetPosition().y);

			/*アニメーション再生。*/
			boss->PlayAnimation(nsActor::BossAnimationID::BiteAttack);
		}

		void BiteAttackStrategy::Update(nsActor::Boss *boss)
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();

			/*攻撃判定と同時にSEを再生。*/
			if (!m_isAttack && m_timer <= 0.7f)
			{
				boss->AttackBite();
				m_isAttack = true;

				/*SE再生。*/
				auto soundManager = FindGO<nsSound::SoundLister>("SoundManager");
				if (soundManager != nullptr && reinterpret_cast<uintptr_t>(soundManager) != 0xFFFFFFFFFFFFFFFF)
				{
					soundManager->GetSEList().PlaySE(nsSound::SE_ID::BiteSoft, 2.0f, false, 100.0f);
				}
			}

			// 攻撃が終わる頃にロック解除
			if (m_timer <= 0.1f)
			{
				boss->UnlockYPosition();
			}
		}

		void BiteAttackStrategy::Exit(nsActor::Boss *boss)
		{
			// 攻撃判定を無効化
			if (boss)
			{
				boss->GetBiteHit().Disable();
				boss->UnlockYPosition();
			}
		}

		bool BiteAttackStrategy::IsEnd()
		{
			return m_timer <= 0.0f;
		}
	}
}