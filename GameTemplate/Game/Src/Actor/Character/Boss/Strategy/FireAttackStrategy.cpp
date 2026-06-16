#include "stdafx.h"
#include "FireAttackStrategy.h"
#include "Src/Actor/Character/Boss/Boss.h"

namespace
{
	const float ATTACK_TIME = 2.5f; //! 1 回の攻撃の継続時間（秒）。この時間が経過すると次の行動に移る。
	const float FIRE_TIME = 1.5f;   //! 火球を発射するタイミング（秒）。攻撃開始からこの時間が経過したら火球を発射する。
}

namespace nsApp
{
	namespace nsAI
	{
		void FireAttackStrategy::Enter(nsActor::Boss *boss)
		{
			m_timer = ATTACK_TIME;
			m_isShot = false;

			/* アニメーションの再生。*/
			boss->PlayAnimation(nsActor::BossAnimationID::FireAttack);
		}

		void FireAttackStrategy::Update(nsActor::Boss *boss)
		{
			/* タイマー更新。*/
			m_timer -= g_gameTime->GetFrameDeltaTime();

			/* タイマー減算処理。*/
			if (!m_isShot && m_timer <= FIRE_TIME)
			{
				boss->ShotFireBall();
				m_isShot = true;
			}
		}

		void FireAttackStrategy::Exit(nsActor::Boss *boss)
		{
			/* 攻撃判定の無効化。*/
			if (boss)
				boss->GetFireHit().Disable();
		}

		bool FireAttackStrategy::IsEnd()
		{
			return m_timer <= 0.0f;
		}
	}
}
