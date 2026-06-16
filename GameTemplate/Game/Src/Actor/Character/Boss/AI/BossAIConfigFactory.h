#pragma once
/**
 * @file   BossAIConfigFactory.h
 * @brief  ボスタイプと戦闘状況から攻撃抽選テーブルを生成するファクトリ。
 * @date   2026/06/15
 */

#include "BossAIConfig.h"
#include "Src/Actor/Character/Common/CharacterModel.h"

namespace nsApp
{
	namespace nsAI
	{
		struct BossTypeParameters;

		/**
		 * @class BossAIConfigFactory
		 * @brief ボスタイプ・性格・TSV パラメータに基づき AttackDistanceTable を構築する。
		 */
		class BossAIConfigFactory
		{
		public:
			/**
			 * @brief ボスタイプと戦闘状況に応じた攻撃距離テーブルを生成する。
			 * @param bossType           ボスタイプ。
			 * @param currentDistance    ターゲットまでの距離。
			 * @param hpRatio            現在 HP 割合（0.0〜1.0）。
			 * @param lastAttackWasBite  直前の攻撃が噛みつきだったか。
			 * @return 攻撃抽選用テーブル。
			 */
			static BossAIConfig::AttackDistanceTable CreateConfigForBoss(CharacterModelType bossType, float currentDistance, float hpRatio, bool lastAttackWasBite);


		private:
			/**
			 * @brief 各ボスタイプ・性格ごとに攻撃距離テーブルを生成するヘルパー関数。
			 * @param distance ターゲットまでの距離。
			 * @param params TSV パラメータ構造体。
			 * @return 攻撃抽選用テーブル。
			 */
			static BossAIConfig::AttackDistanceTable CreateTutorialConfig(float distance, const BossTypeParameters& params);

			/**
			 * @brief グリーンドラゴンの攻撃距離テーブルを生成するヘルパー関数。
			 * @param distance ターゲットまでの距離。
			 * @param params TSV パラメータ構造体。
			 * @return 攻撃抽選用テーブル。
			 */
			static BossAIConfig::AttackDistanceTable CreateGreenDragonConfig(float distance, const BossTypeParameters& params);

			/**
			 * @brief レッドドラゴンの攻撃距離テーブルを生成するヘルパー関数。
			 * @param distance ターゲットまでの距離。
			 * @param hpRatio 現在 HP 割合（0.0〜1.0）。
			 * @param lastAttackWasBite 直前の攻撃が噛みつきだったか。
			 * @param params TSV パラメータ構造体。
			 * @return 攻撃抽選用テーブル。
			 */
			static BossAIConfig::AttackDistanceTable CreateAggressiveConfig(float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params);

			/** 
			 * @param distance ターゲットまでの距離。
			 * @param hpRatio 現在 HP 割合（0.0〜1.0）。
			 * @param lastAttackWasBite 直前の攻撃が噛みつきだったか。
			 * @param params TSV パラメータ構造体。
			 * @return 攻撃抽選用テーブル。
			 */
			static BossAIConfig::AttackDistanceTable CreateBalancedConfig(float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params);

			/**
			 * @param distance ターゲットまでの距離。
			 * @param hpRatio 現在 HP 割合（0.0〜1.0）。
			 * @param lastAttackWasBite 直前の攻撃が噛みつきだったか。
			 * @param params TSV パラメータ構造体。
			 * @return 攻撃抽選用テーブル。
			 */
			static BossAIConfig::AttackDistanceTable CreateDefensiveConfig(float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params);

			/**
			 * @brief トリッキーの攻撃距離テーブルを生成するヘルパー関数。
			 * @param distance ターゲットまでの距離。
			 * @param hpRatio 現在 HP 割合（0.0〜1.0）。
			 * @param lastAttackWasBite 直前の攻撃が噛みつきだったか。
			 * @param params TSV パラメータ構造体。
			 * @return 攻撃抽選用テーブル。
			 */
			static BossAIConfig::AttackDistanceTable CreateTrickyConfig(float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params);
		};
	}
}