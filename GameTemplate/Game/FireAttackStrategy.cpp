#include "stdafx.h"
#include "FireAttackStrategy.h"
#include "Boss.h"
#include "Src/Effect/EffectList.h"

namespace
{
	const float ATTACK_TIME = 2.5f; /*攻撃全体時間。*/
	const float FIRE_TIME = 1.5f;	/*ファイアボール発射タイミング。*/
}

namespace nsApp
{
	namespace nsAI
	{
		void FireAttackStrategy::Enter(nsActor::Boss *boss)
		{
			m_timer = ATTACK_TIME;
			m_isShot = false;

			/*炎攻撃アニメーション。*/
			boss->PlayAnimation(nsActor::BossAnimationID::FireAttack);
		}

		void FireAttackStrategy::Update(nsActor::Boss *boss)
		{
			m_timer -= g_gameTime->GetFrameDeltaTime();

			/*1回だけファイアボール発射。*/
			if (!m_isShot && m_timer <= FIRE_TIME)
			{
				boss->ShotFireBall();
				
				/*ファイアエフェクトを再生。*/
				Vector3 effectPos = boss->GetPosition();
				boss->GetEffectList().PlayEffect(
					nsApp::nsEffect::Effect_ID::Fire,
					effectPos,
					Quaternion::Identity,
					Vector3::One,
					2.0f
				);
				
				m_isShot = true;
			}
		}

		void FireAttackStrategy::Exit(nsActor::Boss *boss)
		{
			// 攻撃判定を無効化
			if (boss)
			{
				boss->GetFireHit().Disable();
			}
		}

		bool FireAttackStrategy::IsEnd()
		{
			return m_timer <= 0.0f;
		}
	}
}
