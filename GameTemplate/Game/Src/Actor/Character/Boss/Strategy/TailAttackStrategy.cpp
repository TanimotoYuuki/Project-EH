#include "stdafx.h"
#include "TailAttackStrategy.h"
#include "Src/Actor/Character/Boss/Boss.h"

#include "Src/Sound/SoundLister.h"
#include "Src/Sound/SEList.h"

namespace
{
	const float ATTACK_TIME = 1.2f;
	const float HIT_TIME = 0.6f;
}

namespace nsApp
{
	namespace nsAI
	{
		void TailAttackStrategy::Enter(nsActor::Boss *boss)
		{
			m_timer = ATTACK_TIME;

			/*攻撃フラグの初期化。*/
			m_isHit = false;

			// 尻尾攻撃のアニメーションを再生
			boss->PlayAnimation(nsActor::BossAnimationID::TailAttack);
		}

		void TailAttackStrategy::Update(nsActor::Boss *boss)
		{
			// タイマーを減らす
			m_timer -= g_gameTime->GetFrameDeltaTime();

			/*一度だけヒット判定を出す。*/
			if (!m_isHit && m_timer <= HIT_TIME)
			{
				m_isHit = true;
				boss->AttackTail();
			}
		}

		void TailAttackStrategy::Exit(nsActor::Boss *boss)
		{
			// ヒット判定を無効化
			if (boss)
				boss->GetTailHit().Disable();
		}

		bool TailAttackStrategy::IsEnd()
		{
			return m_timer <= 0.0f;
		}
	}
}
