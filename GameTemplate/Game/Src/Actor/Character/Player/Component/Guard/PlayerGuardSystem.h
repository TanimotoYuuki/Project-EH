#pragma once

/*
 * @file   PlayerGuardSystem.h
 * @brief  プレイヤーのガードシステムを管理するクラスのヘッダーファイル。
 * @author Yamaguchi Hayato
 * @date   2026/06/21: 初版作成
 */


#include "Src/Actor/Character/Player/Component/Guard/GuardParameter.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	class PlayerGuardSystem
	{
	public:
		/**
		 * @brief ガードシステムを初期化する関数。指定された武器タイプに応じてガードパラメータを設定する。
		 * @param weaponType 初期化する武器タイプ。
		 */
		void Initialize(WeaponType weaponType);

		/**
		 * @brief ガードを開始する関数。ガード時間を最大値に設定し、ガード状態を有効にする。
		 */
		void BeginGuard();

		/**
		 * @brief ガード状態を更新する関数。ガード時間を減少させ、ガードが有効かどうかを判定する。
		 * @param deltaTime 前フレームからの経過時間（秒）。
		 */
		void Update(float deltaTime);

		/**
		 * @brief ガードを終了する関数。ガード状態を無効にし、残り時間をリセットする。
		 */
		inline void EndGuard()
		{
			m_isActive = false;
			m_remainingTime = 0.0f;
		}


	/* ゲッター。*/
	public:
		/**
		 * @brief ガード状態が有効かどうかを判定する関数。
		 * @return ガード状態が有効な場合は true、無効な場合は false を返す。
		 */
		inline bool IsActive() const 
		{
			return m_isActive; 
		}

		/**
		 * @brief ガードが有効で、残り時間が0以下かどうかを判定する関数。
		 * @return ガードが有効で、残り時間が0以下の場合は true、そうでない場合は false を返す。
		 */
		inline bool IsExpired() const 
		{
			return m_remainingTime <= 0.0f; 
		}

		/**
		 * @brief ガードの最大持続時間を取得する関数。
		 * @return ガードの最大持続時間（秒）。
		 */
		inline float GetMaxDuration() const
		{
			return m_param.maxDuration; 
		}

		/**
		 * @brief ガードの残り時間を取得する関数。
		 * @return ガードの残り時間（秒）。
		 */
		inline float GetRemainingTime() const 
		{
			return m_remainingTime; 
		}

		/**
		 * @brief ガードのダメージ軽減率を取得する関数。
		 * @return ガードのダメージ軽減率（0.0～1.0）。
		 */
		inline float GetDamageReduction() const 
		{
			return m_param.damageReduction; 
		}

		/**
		 * @brief ガードが有効で、残り時間が0より大きい場合にゲージを表示するかどうかを判定する関数。
		 * @return ガードが有効で、残り時間が0より大きい場合は true、そうでない場合は false を返す。
		 */
		inline bool IsGaugeVisible() const
		{
			return m_isActive && m_remainingTime > 0.0f;
		}

		/**
		 * @brief 
		 * @return 
		 */
		float GetRemainingRate() const;

		/**
		 * @brief 
		 * @param baseDamage 
		 * @return 
		 */
		int CalculateReducedDamage(int baseDamage) const;

		/**
		 * @brief KnockBackを無効。
		 * @return　条件式。
		 */
		inline bool BlocksKnockback() const
		{
			return m_isActive && m_param.isBlockKnockback;
		}


	private:
		GuardParameter m_param{};		//! ガードパラメータを保持するメンバ変数。

		float m_remainingTime = 0.0f;   //! ガードの残り時間を保持するメンバ変数。

		bool m_isActive = false;	    //! ガード状態が有効かどうかを保持するメンバ変数。
	};
}