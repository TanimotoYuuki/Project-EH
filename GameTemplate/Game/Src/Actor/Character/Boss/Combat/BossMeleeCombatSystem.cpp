#include "stdafx.h"
#include "BossMeleeCombatSystem.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"


namespace
{
	/* 近接攻撃 */
	static constexpr float BITE_ATTACK_FORWARD = 2.0f;
	static constexpr float TAIL_ATTACK_FORWARD = 1.5f;
	static constexpr float BITE_HIT_RADIUS = 20.0f;
	static constexpr float TAIL_HIT_RADIUS = 30.0f;
	static constexpr float MELEE_KNOCKBACK_FORCE = 400.0f;
}


namespace nsApp
{
	namespace nsActor
	{
		void BossMeleeCombatSystem::Init()
		{
			m_BiteHit.Init(BITE_HIT_RADIUS);
			m_TailHit.Init(TAIL_HIT_RADIUS);
		}


		void BossMeleeCombatSystem::AttackBite(Boss* boss)
		{
			/* 前方に噛みつき判定を配置 */
			Vector3 attackPos = boss->GetPosition() + boss->GetForward() * BITE_ATTACK_FORWARD;
			m_BiteHit.Enable();
			m_BiteHit.Update(attackPos);
		}


		Vector3 BossMeleeCombatSystem::AttackTail(Boss* boss)
		{
			/* 前方に尻尾判定を配置 */
			Vector3 attackPos = boss->GetPosition() + boss->GetForward() * TAIL_ATTACK_FORWARD;
			m_TailHit.Enable();
			m_TailHit.Update(attackPos);
			return attackPos;
		}


		void BossMeleeCombatSystem::CheckHitAndDamagePlayers(Boss* boss)
		{
			const auto& targets = boss->GetAllTargets();
			if (targets.empty())
				return;

			const int damage = static_cast<int>(boss->GetCharacterStatus().attack.normalDamage);

			for (auto* target : targets)
			{
				if (target == nullptr || target->IsDead())
					continue;

				bool biteHit = m_BiteHit.IsHit(target);
				bool tailHit = m_TailHit.IsHit(target);
				if (!biteHit && !tailHit)
					continue;

				/* 近接ダメージ適用 */
				DamageProcessor::ApplyDamageToTarget(target, damage);

				/* プレイヤーは吹き飛ばし */
				if (biteHit || tailHit)
				{
					auto* player = dynamic_cast<Player*>(target);
					if (player != nullptr)
					{
						float dirX = (target->GetPosition().x >= boss->GetPosition().x) ? 1.0f : -1.0f;
						player->ForceBlowAway(MELEE_KNOCKBACK_FORCE, dirX);
					}
				}
			}
		}
	}
}