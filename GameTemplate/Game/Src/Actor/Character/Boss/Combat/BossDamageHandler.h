#pragma once

/**
 * @file   BossDamageHandler.h
 * @brief  ボスの被ダメージ累積・硬直（怯み）判定を管理するクラス。
 * @date   2026/06/16
 */

namespace nsApp
{
	namespace nsActor
	{
		class Boss;

		/**
		 * @class BossDamageHandler
		 * @brief 被ダメージの累積と硬直ステート遷移を Boss から委譲する。
		 */
		class BossDamageHandler
		{
		public:
			/**
			 * @brief タイマーと累積ダメージをリセット。Boss::Start() から呼ぶ。
			 */
			void Init();

			/**
			 * @brief 累積ダメージリセット・硬直クールダウンのタイマーを更新。
			 * @param deltaTime 経過時間（秒）。
			 */
			void UpdateTimers(float deltaTime);

			/**
			 * @brief 被ダメージイベントを処理し、必要なら硬直ステートへ遷移させる。
			 * @param boss   被ダメージを受けたボス。
			 * @param damage 今フレームの HP 減少量。
			 */
			void HandleDamageEvent(Boss* boss, int damage);


		private:
			int   m_accumulatedDamage = 0;     //! 硬直判定用累積ダメージ。
			float m_damageResetTimer = 0.0f;   //! 累積ダメージリセットタイマー。
			float m_flinchCooldownTimer = 0.0f; //! 硬直クールダウン。
		};
	}
}