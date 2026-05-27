#include "stdafx.h"
#include "TailAttackStrategy.h"
#include "Boss.h"

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
			m_isHit = false;

			/*尻尾攻撃アニメーション。*/
			boss->PlayAnimation(nsActor::BossAnimationID::TailAttack);
		}

		void TailAttackStrategy::Update(nsActor::Boss *boss)
		{
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
			// 攻撃判定を無効化
			if (boss)
			{
				boss->GetTailHit().Disable();
			}
		}

		bool TailAttackStrategy::IsEnd()
		{
			return m_timer <= 0.0f;
		}
	}
}
