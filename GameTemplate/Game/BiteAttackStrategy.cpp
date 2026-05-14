#include "stdafx.h"
#include "BiteAttackStrategy.h"
#include "Boss.h"

namespace nsApp
{
	namespace nsAI
	{
		void BiteAttackStrategy::Enter(nsActor::Boss* boss)
		{
			/*攻撃時間。*/
			m_timer = 1.0f;

			/*攻撃フラグ初期化。*/
			m_isAttack = false;

			/*Y位置をロック。*/
			boss->LockYPosition(boss->GetPosition().y);

			/*アニメーション再生。*/
			boss->PlayAnimation(nsActor::BossAnimationID::BiteAttack);
		}

		void BiteAttackStrategy::Update(nsActor::Boss* boss)
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();

			if (!m_isAttack && m_timer <= 0.7f)
			{
				boss->AttackBite();
				m_isAttack = true;
			}

			// 攻撃が終わる頃にロック解除
			if (m_timer <= 0.1f)
			{
				boss->UnlockYPosition();
			}
		}

		bool BiteAttackStrategy::IsEnd()
		{
			return m_timer<= 0.0f;
		}
	}
}