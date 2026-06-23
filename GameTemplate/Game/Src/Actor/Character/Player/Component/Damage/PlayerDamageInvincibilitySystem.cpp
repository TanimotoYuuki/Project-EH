#include "stdafx.h"
#include "PlayerDamageInvincibilitySystem.h"

namespace nsApp
{
	void PlayerDamageInvincibilitySystem::Initialize(WeaponType weaponType)
	{
		(void)weaponType;

		/* 将来 DamageInvincibilityParameterTable から読む。*/
		m_param = DamageInvincibilityParameter{};
		m_remainingTime = 0.0f;
	}


	void PlayerDamageInvincibilitySystem::Update(float deltaTime)
	{
		if (m_remainingTime <= 0.0f)
			return;

		m_remainingTime -= deltaTime;
		if (m_remainingTime < 0.0f)
			m_remainingTime = 0.0f;
	}


	void PlayerDamageInvincibilitySystem::BeginAfterKnockBack()
	{
		Begin(m_param.afterKnockBackDuration);
	}


	void PlayerDamageInvincibilitySystem::Begin(float durationSec)
	{
		if (durationSec <= 0.0f)
			return;

		/* 再被弾で延長しない。残りが短いときだけ上書き。*/
		if (m_remainingTime < durationSec)
			m_remainingTime = durationSec;
	}


	void PlayerDamageInvincibilitySystem::End()
	{
		m_remainingTime = 0.0f;
	}


	bool PlayerDamageInvincibilitySystem::IsActive() const
	{
		return m_remainingTime > 0.0f;
	}


	bool PlayerDamageInvincibilitySystem::BlocksDamage() const
	{
		return IsActive();
	}


	bool PlayerDamageInvincibilitySystem::BlocksKnockback() const
	{
		return IsActive();
	}


	float PlayerDamageInvincibilitySystem::GetRemainingTime() const
	{
		return m_remainingTime;
	}
}