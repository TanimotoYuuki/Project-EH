#include "stdafx.h"
#include "DamageIndicatorPool.h"

namespace
{
	constexpr int DAMAGE_TEXT_POOL_SIZE = 32; //! 最大表示数。
}

namespace nsApp
{
	bool DamageIndicatorPool::Start()
	{
		m_damageTextPool.Initialize(DAMAGE_TEXT_POOL_SIZE);
		return true;
	}


	void DamageIndicatorPool::SpawnDamageText(int damageValue, const Vector3& position)
	{
		/* プールからダメージ表示を取得する。*/
		auto* indicator = m_damageTextPool.Spawn();

		if (indicator == nullptr)
			return;

		/* 初期化。*/
		indicator->Init(damageValue, position);
	}
}