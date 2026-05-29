#pragma once
/**
 * @file   RescueStatusManager.h
 * @brief  救助状況管理クラス
 * @author Yamaguchi Hayato
 * @date   2026/05/26
 */

#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	class RescueStatusManager
	{
	/* セッター。*/
	public:
		/**
		 * @brief 救助されているかどうかを設定する。
		 * @param flag 救助されているかどうかのフラグ。
	     * true : 救助されている
		 * false: 救助されていない
		 */
		inline void SetBiingHelped(bool flag)
		{
			m_isBeingHelped = flag;
		}


	public:
		/**
		 * @brief ダウンした回数を加算する。
		 */
		inline void AddDownCount()
		{
			m_downCount++;
		}

		/**
		 * @brief 救助されているかどうかをリセットする。
		 */
		inline void ResetRescueStatus()
		{
			m_isBeingHelped = false;
		}

		/**
		 * @brief 救助に必要な時間を計算する。
		 * @param helperWeapon 救助する武器の種類。
		 * @return 救助に必要な時間。
		 */
		int CalculateRequiredHelpTime(nsApp::WeaponType helperWeapon) const;
		

	/* ゲッター。*/
	public:
		/**
		 * @brief ダウンした回数を取得する。
		 * @return ダウンした回数。
		 */
		inline int GetDownCount()
		{
			return m_downCount;
		}

		/**
		 * @brief 救助されているかどうかを取得する。
		 * @return 救助されているかどうか。
		 * true : 救助されている
		 */
		inline bool IsBeingHelped() const 
		{
			return m_isBeingHelped; 
		}


	private:
		int m_requiredTime = 0;		   //! 救助に必要な時間
		int m_downCount = 0;		   //! ダウンした回数

		float m_multiplier = 0.0f;	   //! ダウン回数に応じた時間の倍率

		bool m_isBeingHelped = false;  //! 救助されているかどうか
	};
}
