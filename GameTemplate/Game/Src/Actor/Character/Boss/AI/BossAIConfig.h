#pragma once
/**
 * @file   BossAIConfig.h
 * @brief  ボス AI の共通定数と攻撃抽選テーブル型を定義する。
 * @date   2026/06/15
 */

namespace nsApp
{
	namespace nsAI
	{
		/**
		 * @struct BossAIConfig
		 * @brief  ボス AI の距離・HP・咆哮・怯みに関する定数を集約する。
		 * @note   インスタンス化しない。定数の名前空間として使用する。
		 *         攻撃確率テーブルの生成は BossAIConfigFactory が担当する。
		 */
		struct BossAIConfig
		{
			/* ============================================================
			 * 攻撃距離の閾値（Idle / Move の行動判断用）
			 * ============================================================ */
			static constexpr float CLOSE_ATTACK_DISTANCE = 35.0f;  //! 近距離の上限。
			static constexpr float MEDIUM_ATTACK_DISTANCE = 90.0f; //! 中距離の上限。
			static constexpr float FAR_DISTANCE = 150.0f;          //! 遠距離の目安。
			static constexpr float MOVE_STOP_DISTANCE = 50.0f;     //! 移動停止して攻撃する距離。

			/* ============================================================
			 * 攻撃抽選テーブル用の距離境界
			 * @note Idle 用の CLOSE/MEDIUM とは別系統。値を変更しないこと。
			 *       BossAIConfigFactory でも使用する。
			 * ============================================================ */
			static constexpr float ATTACK_TABLE_NEAR_MAX = 25.0f;   //! 近距離帯の上限。
			static constexpr float ATTACK_TABLE_MID_MAX = 55.0f;    //! 中距離帯の上限。
			static constexpr float ATTACK_TABLE_FAR_MAX = 1000.0f;  //! 遠距離帯の上限（実質無限）。

			/* ============================================================
			 * 行動タイミング（デフォルト値。TSV で上書きされる場合あり）
			 * ============================================================ */
			static constexpr float IDLE_TIME = 2.0f;			    //! 待機時間（秒）。
			static constexpr float MOVE_DURATION = 3.0f;		    //! 移動継続時間（秒）。

			/* ============================================================
			 * HP 割合による行動変化
			 * ============================================================ */
			static constexpr float HIGH_HP_RATIO = 0.7f;		    //! 高 HP とみなす割合。
			static constexpr float LOW_HP_RATIO = 0.3f;				//! 低 HP とみなす割合。

			/* ============================================================
			 * フェーズ移行（Boss::CheckAndApplyPhase / 演出イベント共用）
			 * ============================================================ */
			static constexpr float PHASE1_HP_THRESHOLD = 0.5f;		 //! フェーズ1移行 HP 割合。
			static constexpr float PHASE2_HP_THRESHOLD = 0.25f;		 //! フェーズ2移行 HP 割合。

			/* ============================================================
			 * 被ダメージ・怯み
			 * ============================================================ */
			static constexpr int   FLINCH_DAMAGE_THRESHOLD = 500;   //! 怯み発生の累積ダメージ。
			static constexpr float DAMAGE_RESET_TIME = 5.0f;        //! ダメージ蓄積リセット（秒）。
			static constexpr float FLINCH_COOLDOWN = 2.0f;          //! 怯みクールダウン（秒）。

			/* ============================================================
			 * 咆哮（Roar）
			 * ============================================================ */
			static constexpr float ROAR_HP_THRESHOLD_FIRST = 0.6f;  //! 1 回目咆哮 HP 閾値。
			static constexpr float ROAR_HP_THRESHOLD_SECOND = 0.3f; //! 2 回目咆哮 HP 閾値。
			static constexpr float ROAR_COOLDOWN = 15.0f;           //! 咆哮クールダウン（秒）。
			static constexpr int   ROAR_MOVE_CHANCE = 65;           //! 遠距離で Move を選ぶ確率（%）。
			static constexpr int   ROAR_ATTACK_CHANCE = 35;         //! 遠距離で Roar 以外を選ぶ際の参考値（%）。

			/**
			 * @struct AttackDistanceTable
			 * @brief  距離帯ごとの攻撃抽選確率（0〜99）を保持する。
			 * @note   bite + tail + fire は 100 になる必要はない（残りは未使用扱い）。
			 */
			struct AttackDistanceTable
			{
				float minDistance; //! 距離帯の下限。
				float maxDistance; //! 距離帯の上限。
				int   biteChance;  //! 噛みつき確率。
				int   tailChance;  //! 尻尾確率。
				int   fireChance;  //! 火炎確率。
			};
		};
	}
}