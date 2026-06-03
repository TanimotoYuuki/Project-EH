#include "stdafx.h"
#include "RescueStatusLister.h"
#include <unordered_map>

namespace
{
	const int BASE_HELP_TIME = 90;          //! 基本救助時間。60FPSなら1.5秒。
	const int PENALTY_TIME_PER_DOWN = 30;   //! ダウン1回ごとの追加時間。60FPSなら0.5秒。

	/**
	 * @brief 武器ごとの救助時間倍率。
	 * 小さいほど救助が速い。
	 */
	const std::unordered_map<nsApp::WeaponType, float> JOB_SPEED_MAP =
	{
		{ nsApp::WeaponType::GreatSword, 1.0f },
		{ nsApp::WeaponType::Hammer,     1.2f },
		{ nsApp::WeaponType::TwinGun,    0.8f },
		{ nsApp::WeaponType::Wand,       0.5f },
		{ nsApp::WeaponType::None,       1.0f }
	};
}

namespace nsApp
{
	int RescueStatusLister::CalculateRequiredHelpTime(WeaponType helperWeapon) const
	{
		/* ダウン回数に応じて基本時間にペナルティを加算。*/ 
		int requiredTime = BASE_HELP_TIME + (m_downCount * PENALTY_TIME_PER_DOWN);

		/* 武器種に応じて時間を倍率変換。*/
		const auto it = JOB_SPEED_MAP.find(helperWeapon);
		const float multiplier = (it != JOB_SPEED_MAP.end()) ? it->second : 1.0f;

		/* 救助時間に倍率を適用。*/
		requiredTime = static_cast<int>(requiredTime * multiplier);

		/* 最低でも1フレームは必要。*/
		if (requiredTime <= 0)
			requiredTime = 1;

		/* 計算された救助時間を返す。*/
		return requiredTime;
	}


	void RescueStatusLister::BeginHelp(WeaponType helperWeapon)
	{
		m_isBeingHelped = true;
		m_helpTimer = 0;
		m_requiredHelpTime = CalculateRequiredHelpTime(helperWeapon);

		if (m_requiredHelpTime <= 0)
			m_requiredHelpTime = 1;
	}


	bool RescueStatusLister::UpdateHelpProgress()
	{
		if (!m_isBeingHelped)
			return false;

		if (m_requiredHelpTime <= 0)
			return false;

		m_helpTimer++;

		return m_helpTimer >= m_requiredHelpTime;
	}


	float RescueStatusLister::GetHelpRate() const
	{
		/* 救助が開始されていない、もしくは必要な救助時間が0以下の場合は0を返す。*/
		if (m_requiredHelpTime <= 0)
			return 0.0f;

		/* 救助の進行率を計算。0.0f ～ 1.0f の範囲にクランプする。*/
		const float rate = static_cast<float>(m_helpTimer) / static_cast<float>(m_requiredHelpTime);

		/* クランプ処理。0.0f未満は0.0f、1.0fを超える場合は1.0fにする。*/
		if (rate < 0.0f)
			return 0.0f;
		if (rate > 1.0f)
			return 1.0f;

		/* 計算された進行率を返す。*/
		return rate;
	}
}