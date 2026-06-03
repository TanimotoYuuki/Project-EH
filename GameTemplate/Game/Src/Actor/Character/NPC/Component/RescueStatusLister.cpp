#include "stdafx.h"
#include "RescueStatusLister.h"

namespace
{
	const int BASE_REQUIRED_HELP_TIME = 90; //! 救助に必要な基本時間（秒）
	const int PENALTY_TIME_PER_DOWN = 30;   //! ダウンするごとに追加されるペナルティ時間（秒）

	const auto HELPER_RATE_COMPARISON = 0;  //! 救助率の条件比較用。
	const auto HELPER_RATE_CLIPPING = 1;    //! 救助率のクリッピング用。

	const auto HELPER_RATE = 0.0f;		    //! 救助率の条件比較用。
	const auto HELPER_RATE_MAX = 1.0f;	    //! 救助率の最大値。

	const auto MOVE_SPEED_SWORD = 1.0f;	    //! 剣の移動速度倍率。
	const auto MOVE_SPEED_HAMMER = 1.2f;    //! ハンマーの移動速度倍率。
	const auto MOVE_SPEED_TWIN_GUN = 0.8f;  //! 双銃の移動速度倍率。
	const auto MOVE_SPEED_WAND = 0.5f;      //! 杖の移動速度倍率。
	const auto MOVE_SPEED_DEFAULT = 1.0f;   //! デフォルトの移動速度倍率。
}

namespace nsApp
{
	namespace
	{

		/**
		 * @brief 武器ごとの救助速度の倍率を取得する関数
		 * @param helperWeapon　救助に使用される武器の種類
		 * @return　武器ごとの救助速度の倍率
		 */
		float GetJobSpeedMultiplier(WeaponType helperWeapon)
		{
			switch (helperWeapon)
			{
			case WeaponType::GreatSword:
				return MOVE_SPEED_SWORD;
			case WeaponType::Hammer:
				return MOVE_SPEED_HAMMER;
			case WeaponType::TwinGun:
				return MOVE_SPEED_TWIN_GUN;
			case WeaponType::Wand:
				return MOVE_SPEED_WAND;
			default:
				return MOVE_SPEED_DEFAULT;
			}
		}
	}


	int RescueStatusLister::CalculateRequiredHelpTime(WeaponType helperWeapon) const
	{
		/* 基本時間にダウン数に応じたペナルティ時間を加算し、さらに武器ごとの速度倍率を適用して必要な救助時間を計算。*/
		const int baseTime = BASE_REQUIRED_HELP_TIME + (m_downCount * PENALTY_TIME_PER_DOWN);
		const float speedMultiplier = GetJobSpeedMultiplier(helperWeapon);

		/* 計算結果が0以下にならないように最低1秒は必要とする。 */
		int requiredTime = static_cast<int>(static_cast<float>(baseTime) * speedMultiplier);
		if (requiredTime <= 0)
			requiredTime = 1;

		return requiredTime;
	}


	void RescueStatusLister::BeginHelp(WeaponType helperWeapon)
	{
		/* 救助開始時に必要な情報を初期化し、救助時間を計算する。*/
		m_isBeingHelped = true;
		/* 救助に使用される武器の種類を保存。*/
		m_helpTimer = 0;
		/* 救助に必要な時間を計算。*/
		m_requiredHelpTime = CalculateRequiredHelpTime(helperWeapon);

		/* 計算結果が0以下にならないように最低1秒は必要とする。*/
		if (m_requiredHelpTime <= HELPER_RATE)
			m_requiredHelpTime = HELPER_RATE_MAX;
	}


	bool RescueStatusLister::UpdateHelpProgress()
	{
		/* 救助中でない場合や必要な救助時間が0以下の場合は進行を更新せずにfalseを返す。*/
		if (!m_isBeingHelped)
			return false;

		/* 救助時間を1フレーム分進める。*/
		if (m_requiredHelpTime <= HELPER_RATE)
			return false;

		/* 救助時間が必要な時間に達したかどうかを判断する。*/
		m_helpTimer++;
		return m_helpTimer >= m_requiredHelpTime;
	}


	float RescueStatusLister::GetHelpRate() const
	{
		/* 救助に必要な時間が0以下の場合は救助率を0%とする。*/
		if (m_requiredHelpTime <= HELPER_RATE)
			return HELPER_RATE;

		/* 救助時間が必要な時間に対してどれだけ進んでいるかを計算し、0.0fから1.0fの範囲で返す。*/
		const float rate = static_cast<float>(m_helpTimer) / static_cast<float>(m_requiredHelpTime);

		/* 計算結果が0.0f未満や1.0fを超えないようにクランプする。*/
		if (rate < HELPER_RATE_COMPARISON)
			return HELPER_RATE_COMPARISON;
		if (rate > HELPER_RATE_CLIPPING)
			return HELPER_RATE_CLIPPING;

		/* 救助率を返す。*/
		return rate;
	}
}
