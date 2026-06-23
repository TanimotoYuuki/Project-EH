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
		/* インターバルフレーム数は攻撃の激しさ（aggression）が高いほど短くなる。*/
		if (intervalFrame < 0)
			intervalFrame = 0;
		if (aggression <= 0.0f)
			aggression = 1.0f;

		/* 攻撃の激しさを考慮してインターバルフレーム数を計算する。*/
		m_attackIntervalFrame = static_cast<int>(static_cast<float>(intervalFrame) / aggression);
	}


	void NPCCombatMind::StartAttackInterval()
	{
		/* 攻撃終了後、再攻撃まで待つ。*/
		m_attackIntervalTimer = m_attackIntervalFrame;
	}


	void NPCCombatMind::UpdateAttackInterval()
	{
		/* インターバルタイマーがリファレンス値以下なら、攻撃可能な状態なので更新しない。*/
		if (m_attackIntervalTimer <= REFERENCE_VALUE_ATTACK_INTERVAL)
			return;

		/* インターバルタイマーを減少させる。*/
		m_attackIntervalTimer--;

		/* インターバルタイマーがリファレンス値以下になったら、攻撃可能な状態にする。*/
		if (m_attackIntervalTimer < REFERENCE_VALUE_ATTACK_INTERVAL)
			m_attackIntervalTimer = REFERENCE_VALUE_ATTACK_INTERVAL;
	}


	bool NPCCombatMind::CanAttack() const
	{
		/* インターバルタイマーがリファレンス値以下なら、攻撃可能な状態。*/
		return m_attackIntervalTimer <= 0;
	}


	bool NPCCombatMind::IsBossAttackWindow() const
	{
		/* Bossクラスの探索処理。*/
		auto* boss = FindGO<nsActor::Boss>("boss");
		if (boss == nullptr)
			return false;

		/* 被ダメ中は硬直 → 攻撃チャンス。*/
		return boss->GetCurrentStateID() == static_cast<uint8_t>(nsActor::BossStateID::enDamage);
	}
}