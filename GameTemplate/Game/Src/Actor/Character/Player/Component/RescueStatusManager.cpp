#include "stdafx.h"
#include "RescueStatusManager.h"

namespace
{
	const auto BASE_HELP_TIME = 30;		   //! 助けるのに必要な時間。
	const auto PENALTY_TIME_PER_DOWN = 10; //! ダウンするごとに加算されるペナルティ時間。

	const std::unordered_map<nsApp::WeaponType, float> RESCUE_TIME_MULTIPLIER =
	{
		{nsApp::WeaponType::GreatSword, 10.0f},
		{nsApp::WeaponType::Hammer,15.0f},
		{nsApp::WeaponType::TwinGun, 12.0f},
		{nsApp::WeaponType::Wand, 8.0f}
	};
}

namespace nsApp
{
	int RescueStatusManager::CalculateRequiredHelpTime(WeaponType helperWeapon) const
	{
		/* 基本の助けるのに必要な時間に、ダウンするごとに加算されるペナルティ時間を加算する。*/
		int requiredTime = BASE_HELP_TIME + (m_downCount * PENALTY_TIME_PER_DOWN);

		/* 助ける武器の種類に応じた時間の倍率を適用する。*/
		float multiplier = RESCUE_TIME_MULTIPLIER.count(helperWeapon) ? RESCUE_TIME_MULTIPLIER.at(helperWeapon) : 1.0f;

		/* 計算された助けるのに必要な時間を返す。*/
		return static_cast<int>(requiredTime * multiplier);
	}
}