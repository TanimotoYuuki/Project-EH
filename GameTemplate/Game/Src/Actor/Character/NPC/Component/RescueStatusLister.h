#pragma once

#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	class RescueStatusLister
	{
	public:
		/**
		 * @brief ダウン回数を加算する。
		 */
		inline void AddDownCount()
		{
			m_downCount++;
		}

		/**
		 * @brief 救助に必要な時間を計算する。
		 * @param helperWeapon 救助する側の武器種。
		 * @return 救助完了に必要なフレーム数。
		 */
		int CalculateRequiredHelpTime(WeaponType helperWeapon) const;

		/**
		 * @brief 救助状態をリセットする。
		 */
		inline void ResetRescueStatus()
		{
			m_isBeingHelped = false;
			m_helpTimer = 0;
			m_requiredHelpTime = 0;
		}

		/**
		 * @brief 救助を開始する。
		 * @param helperWeapon 救助する側の武器種。
		 */
		void BeginHelp(WeaponType helperWeapon);

		/**
		 * @brief 救助進行を更新する。
		 * @return 救助が完了した場合true。
		 */
		bool UpdateHelpProgress();

		/**
		 * @brief 救助を中断する。
		 */
		inline void CancelHelp()
		{
			m_isBeingHelped = false;
			m_helpTimer = 0;
			m_requiredHelpTime = 0;
		}

		/**
		 * @brief 救助進行率を取得する。
		 * @return 0.0f ～ 1.0f の救助進行率。
		 */
		float GetHelpRate() const;


		/* セッター。*/
	public:
		/**
		 * @brief 救助中かどうかを設定する。
		 * @param flag 救助中ならtrue。
		 */
		inline void SetBeingHelped(bool flag)
		{
			m_isBeingHelped = flag;
		}


		/* ゲッター。*/
	public:
		/**
		 * @brief 救助中かどうかを取得する。
		 * @return 救助中ならtrue。
		 */
		inline bool IsBeingHelped() const
		{
			return m_isBeingHelped;
		}

		/**
		 * @brief ダウン回数を取得する。
		 * @return ダウン回数。
		 */
		inline int GetDownCount() const
		{
			return m_downCount;
		}

		/**
		 * @brief 救助経過フレームを取得する。
		 * @return 救助経過フレーム。
		 */
		inline int GetHelpTimer() const
		{
			return m_helpTimer;
		}

		/**
		 * @brief 救助完了に必要なフレームを取得する。
		 * @return 救助完了に必要なフレーム。
		 */
		inline int GetRequiredHelpTime() const
		{
			return m_requiredHelpTime;
		}


	private:
		int m_downCount = 0;           //! ダウン回数。
		int m_helpTimer = 0;           //! 救助経過フレーム。
		int m_requiredHelpTime = 0;    //! 救助完了に必要なフレーム。

		bool m_isBeingHelped = false;  //! 救助中かどうか。
	};
}