#include "stdafx.h"
#include "RescueStatusManager.h"
#include "Src/Actor/Character/NPC/Component/RescueTimeTable.h"

namespace
{
	const auto PENALTY_TIME_PER_DOWN = 10;
	const auto REQUEST_TIME = 1.0f;
}

namespace nsApp
{
	int RescueStatusManager::CalculateRequiredHelpTime(WeaponType helperWeapon) const
	{
		/* 救助に必要な時間を計算する。*/
		const auto& parameter = RescueTimeTable::GetParameter(helperWeapon);

		/* ダウン数に応じたペナルティ時間を加算する。*/
		float requiredTime = static_cast<float>(parameter.requiredHelpTime + (m_downCount * PENALTY_TIME_PER_DOWN));
		if (parameter.rescueGaugeSpeed > 0.0f)
			requiredTime /= parameter.rescueGaugeSpeed;

		/* リクエストの要請。*/
		if (requiredTime <= REQUEST_TIME)
			requiredTime = REQUEST_TIME;

		return static_cast<int>(requiredTime);
	}
}
