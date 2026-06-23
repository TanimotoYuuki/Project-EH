#include "stdafx.h"
#include "NPCCombatMind.h"
#include "Src/Actor/Character/Boss/Boss.h"

namespace
{
	const auto REFERENCE_VALUE_ATTACK_INTERVAL = 0; //! インターバル比較用。
}

namespace nsApp
{
	void NPCCombatMind::SetAttackInterval(int intervalFrame, float aggression)
	{
		if (intervalFrame < 0)
			intervalFrame = 0;

		if (aggression <= 0.0f)
			aggression = 1.0f;

		m_attackIntervalFrame = static_cast<int>(static_cast<float>(intervalFrame) / aggression);
	}


	void NPCCombatMind::StartAttackInterval()
	{
		/* 攻撃終了後、再攻撃まで待つ。*/
		m_attackIntervalTimer = m_attackIntervalFrame;
	}


	void NPCCombatMind::UpdateAttackInterval()
	{
		if (m_attackIntervalTimer <= REFERENCE_VALUE_ATTACK_INTERVAL)
			return;

		m_attackIntervalTimer--;

		if (m_attackIntervalTimer < REFERENCE_VALUE_ATTACK_INTERVAL)
			m_attackIntervalTimer = REFERENCE_VALUE_ATTACK_INTERVAL;
	}


	bool NPCCombatMind::CanAttack() const
	{
		return m_attackIntervalTimer <= 0;
	}


	bool NPCCombatMind::IsBossAttackWindow() const
	{
		auto* boss = FindGO<nsActor::Boss>("boss");
		if (boss == nullptr)
			return false;

		/* 被ダメ中は硬直 → 攻撃チャンス。*/
		return boss->GetCurrentStateID() == static_cast<uint8_t>(nsActor::BossStateID::enDamage);
	}
}