#pragma once

/**
 * @file   BossPhaseController.h
 * @brief  ボスの HP フェーズに応じたステータス強化を管理するクラス。
 * @date   2026/06/16
 */

namespace nsApp
{
	namespace nsActor
	{
		class Boss;

		/**
		 * @class BossPhaseController
		 * @brief HP 閾値到達時の攻撃力・待機時間強化を Boss から委譲する。
		 */
		class BossPhaseController
		{
		public:
			/**
			 * @brief フェーズ状態をリセット。Boss::Start() から呼ぶ。
			 */
			void Init();

			/**
			 * @brief HP 比率を確認し、未発火のフェーズ強化を適用する。
			 * @param boss 対象ボス。
			 */
			void CheckAndApplyPhase(Boss* boss);

			/**
			 * @brief フェーズ補正後の待機時間倍率を取得。
			 * @return 待機時間倍率。
			 */
			inline float GetIdleTimeMultiplier() const
			{
				return m_idleTimeMultiplier; 
			}

			/**
			 * @brief フェーズ1強化が発火済みか。
			 * @return true なら発火済み。
			 */
			inline bool IsPhase1Triggered() const 
			{
				return m_phase1Triggered; 
			}


		private:
			bool  m_phase1Triggered = false;		  //! フェーズ1発火済み。
			bool  m_phase2Triggered = false;		  //! フェーズ2発火済み。
			float m_idleTimeMultiplier = 1.0f;        //! 待機時間倍率。

			float m_phase1DamageMultiplier = 1.3f;    //! フェーズ1攻撃力倍率。
			float m_phase2DamageMultiplier = 1.3f;    //! フェーズ2攻撃力倍率。
			float m_phase1IdleTimeMultiplier = 0.75f; //! フェーズ1待機時間倍率。
			float m_phase2IdleTimeMultiplier = 0.5f;  //! フェーズ2待機時間倍率。
		};
	}
}