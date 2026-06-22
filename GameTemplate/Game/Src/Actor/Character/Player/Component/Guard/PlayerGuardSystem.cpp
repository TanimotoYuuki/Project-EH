#include "stdafx.h"
#include "PlayerGuardSystem.h"
#include "Src/Actor/Character/Player/Component/Guard/GuardParameterTable.h"

namespace nsApp
{
	void PlayerGuardSystem::Initialize(WeaponType weaponType)
	{
		m_param = GuardParameterTable::GetParameter(weaponType);
		m_remainingTime = 0.0f;
		m_isActive = false;
	}


	void PlayerGuardSystem::BeginGuard()
	{
		m_isActive = true;
		m_remainingTime = m_param.maxDuration;
	}


	void PlayerGuardSystem::Update(float deltaTime)
	{
		if (!m_isActive)
			return;

		m_remainingTime -= deltaTime;
		if (m_remainingTime < 0.0f)
			m_remainingTime = 0.0f;
	}


	float PlayerGuardSystem::GetRemainingRate() const
	{
		if (m_param.maxDuration <= 0.0f)
			return 0.0f;

		float rate = m_remainingTime / m_param.maxDuration;
		if (rate < 0.0f) return 0.0f;
		if (rate > 1.0f) return 1.0f;
		return rate;
	}


	int PlayerGuardSystem::CalculateReducedDamage(int baseDamage) const
	{
		if (!m_isActive || baseDamage <= 0)
			return baseDamage;

		const float reduced = static_cast<float>(baseDamage) * (1.0f - m_param.damageReduction);
		return static_cast<int>(reduced);
	}
}