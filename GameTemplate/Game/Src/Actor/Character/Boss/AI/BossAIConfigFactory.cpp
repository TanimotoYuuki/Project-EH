#include "stdafx.h"
#include "BossAIConfigFactory.h"
#include "Src/Actor/Character/Boss/Status/BossManager.h"

namespace
{
	static constexpr float NEAR_MAX = 30.0f; //! Aggressive 専用の近距離境界。Balanced や Defensive よりも狭め。
	static constexpr float MID_MAX = 60.0f;	 //! Aggressive 専用の中距離境界。Balanced よりは広めだが、Defensive よりは狭め。
}


namespace nsApp
{
	namespace nsAI
	{
		using Table = BossAIConfig::AttackDistanceTable;

		
		Table BossAIConfigFactory::CreateConfigForBoss(CharacterModelType bossType, float currentDistance, float hpRatio, bool lastAttackWasBite)
		{
			/* ボスタイプに基づいて攻撃距離テーブルを生成する。*/
			const BossTypeParameters& params = BossTypeManager::GetBossTypeParameters(bossType);

			/* チュートリアルボスとグリーンドラゴンは専用のテーブルを使用する。*/
			if (bossType == CharacterModelType::TutorialBoss)
				return CreateTutorialConfig(currentDistance, params);
			/* グリーンドラゴンは火炎攻撃を使用しないため、専用のテーブルを使用する。*/
			if (bossType == CharacterModelType::GreenDragon)
				return CreateGreenDragonConfig(currentDistance, params);


			/* それ以外のボスタイプは性格に応じたテーブルを使用する。*/
			switch (params.personality)
			{
			/* 性格ごとに攻撃距離テーブルを生成する。*/
			case BossPersonality::Aggressive:
				/* Aggressive は距離に対してより攻撃的なテーブルを使用する。*/
				return CreateAggressiveConfig(currentDistance, hpRatio, lastAttackWasBite, params);

			case BossPersonality::Defensive:
				/* Defensive は距離に対してより防御的なテーブルを使用する。*/
				return CreateDefensiveConfig(currentDistance, hpRatio, lastAttackWasBite, params);

			case BossPersonality::Tricky:
				/* Tricky は距離に対してよりトリッキーなテーブルを使用する。*/
				return CreateTrickyConfig(currentDistance, hpRatio, lastAttackWasBite, params);

			case BossPersonality::Balanced:
			default:
				/* Balanced は距離に対してバランスの取れたテーブルを使用する。*/
				return CreateBalancedConfig(currentDistance, hpRatio, lastAttackWasBite, params);
			}
		}

		Table BossAIConfigFactory::CreateTutorialConfig( float distance, const BossTypeParameters& params)
		{
			/* チュートリアルボスは距離に応じて攻撃の優先度が変化するが、性格や HP 割合には影響されない。*/
			const int bite = params.m_preferredBiteChance; //! TSV パラメータから噛みつきの優先度を取得
			const int tail = params.m_preferredTailChance; //! TSV パラメータから尻尾攻撃の優先度を取得
			const int fire = params.m_preferredFireChance; //! TSV パラメータから火炎攻撃の優先度を取得

			using C = BossAIConfig;

			/* 距離に応じて攻撃の優先度を調整する。近距離では噛みつきと尻尾攻撃を優先し、遠距離では火炎攻撃を優先する。*/
			if (distance < C::ATTACK_TABLE_NEAR_MAX)
				return { 0.0f, C::ATTACK_TABLE_NEAR_MAX, bite + 8, tail, fire > 8 ? fire - 8 : fire };

			/* 中距離では噛みつきの優先度を少し下げ、尻尾攻撃と火炎攻撃の優先度を少し上げる。*/
			if (distance < C::ATTACK_TABLE_MID_MAX)
				return { C::ATTACK_TABLE_NEAR_MAX, C::ATTACK_TABLE_MID_MAX, bite, tail, fire };

			/* 遠距離では噛みつきの優先度を大幅に下げ、尻尾攻撃の優先度を維持し、火炎攻撃の優先度を大幅に上げる。*/
			return { C::ATTACK_TABLE_MID_MAX, C::ATTACK_TABLE_FAR_MAX, bite > 8 ? bite - 8 : bite, tail, fire + 8 };
		}


		Table BossAIConfigFactory::CreateAggressiveConfig( float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params)
		{
			/* Aggressive は距離に対してより攻撃的なテーブルを使用する。HP 割合や直前の攻撃は考慮しないが、TSV パラメータは考慮する。*/
			(void)hpRatio;
			(void)params;

			/* Aggressive 専用の距離境界（30 / 60）。他性格とは意図的に異なる。*/
			if (distance < NEAR_MAX)
				return { 0.0f, NEAR_MAX, lastAttackWasBite ? 50 : 80, 15, 5 };

			/* 中距離では噛みつきの優先度を少し下げ、尻尾攻撃の優先度を維持し、火炎攻撃の優先度を少し上げる。*/
			if (distance < MID_MAX)
				return { NEAR_MAX, MID_MAX, lastAttackWasBite ? 40 : 70, 20, 10 };

			/* 遠距離では噛みつきの優先度をさらに下げ、尻尾攻撃の優先度を少し上げ、火炎攻撃の優先度をさらに上げる。*/
			return { MID_MAX, BossAIConfig::ATTACK_TABLE_FAR_MAX, 60, 30, 10 };
		}


		Table BossAIConfigFactory::CreateBalancedConfig( float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params)
		{
			/* Balanced は距離に対してバランスの取れたテーブルを使用する。HP 割合や直前の攻撃は考慮しないが、TSV パラメータは考慮する。*/
			(void)hpRatio;
			(void)lastAttackWasBite;
			(void)params;

			/* Balanced 専用の距離境界（40 / 70）。Aggressive よりは広めだが、Defensive よりは狭め。*/
			using C = BossAIConfig;

			/* 距離に応じて攻撃の優先度を調整する。近距離では噛みつきと尻尾攻撃を優先し、遠距離では火炎攻撃を優先する。Aggressive よりはバランスの取れた数値。*/
			if (distance < C::ATTACK_TABLE_NEAR_MAX)
				return { 0.0f, C::ATTACK_TABLE_NEAR_MAX, 40, 30, 30 };

			/* 中距離では噛みつきの優先度を少し下げ、尻尾攻撃と火炎攻撃の優先度を少し上げる。Aggressive よりはバランスの取れた数値。*/
			if (distance < C::ATTACK_TABLE_MID_MAX)
				return { C::ATTACK_TABLE_NEAR_MAX, C::ATTACK_TABLE_MID_MAX, 25, 40, 35 };

			/* 遠距離では噛みつきの優先度をさらに下げ、尻尾攻撃の優先度を少し上げ、火炎攻撃の優先度をさらに上げる。Aggressive よりはバランスの取れた数値。*/
			return { C::ATTACK_TABLE_MID_MAX, C::ATTACK_TABLE_FAR_MAX, 15, 20, 65 };
		}


		Table BossAIConfigFactory::CreateDefensiveConfig(float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params)
		{
			/* Defensive は距離に対してより防御的なテーブルを使用する。HP 割合や直前の攻撃は考慮しないが、TSV パラメータは考慮する。*/
			(void)hpRatio;
			(void)lastAttackWasBite;
			(void)params;

			/* Defensive 専用の距離境界（50 / 80）。Aggressive よりは広めで、Balanced よりもさらに広め。*/
			using C = BossAIConfig;

			/* 距離に応じて攻撃の優先度を調整する。近距離では噛みつきの優先度を少し上げ、尻尾攻撃の優先度を維持し、火炎攻撃の優先度を少し下げる。Aggressive よりは防御的な数値。*/
			if (distance < C::ATTACK_TABLE_NEAR_MAX)
				return { 0.0f, C::ATTACK_TABLE_NEAR_MAX, 50, 30, 20 };

			/* 中距離では噛みつきの優先度を維持し、尻尾攻撃の優先度を少し下げ、火炎攻撃の優先度を少し上げる。Aggressive よりは防御的な数値。*/
			if (distance < C::ATTACK_TABLE_MID_MAX)
				return { C::ATTACK_TABLE_NEAR_MAX, C::ATTACK_TABLE_MID_MAX, 30, 40, 30 };

			/* 遠距離では噛みつきの優先度をさらに下げ、尻尾攻撃の優先度を少し上げ、火炎攻撃の優先度をさらに上げる。Aggressive よりは防御的な数値。*/
			return { C::ATTACK_TABLE_MID_MAX, C::ATTACK_TABLE_FAR_MAX, 20, 30, 50 };
		}


		Table BossAIConfigFactory::CreateTrickyConfig(float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters& params)
		{
			/* Tricky は距離に対してよりトリッキーなテーブルを使用する。HP 割合や直前の攻撃は考慮しないが、TSV パラメータは考慮する。*/
			(void)hpRatio;
			(void)lastAttackWasBite;
			(void)params;

			/* Tricky 専用の距離境界（20 / 50）。Aggressive よりは狭めで、Defensive よりは広め。*/
			using C = BossAIConfig;

			/* 距離に応じて攻撃の優先度を調整する。近距離では噛みつきと尻尾攻撃を優先し、遠距離では火炎攻撃を優先する。Aggressive よりはトリッキーな数値。*/
			if (distance < C::ATTACK_TABLE_NEAR_MAX)
				return { 0.0f, C::ATTACK_TABLE_NEAR_MAX, 20, 60, 20 };

			/* 中距離では噛みつきの優先度を少し下げ、尻尾攻撃の優先度を大幅に上げ、火炎攻撃の優先度を少し下げる。Aggressive よりはトリッキーな数値。*/
			if (distance < C::ATTACK_TABLE_MID_MAX)
				return { C::ATTACK_TABLE_NEAR_MAX, C::ATTACK_TABLE_MID_MAX, 15, 65, 20 };

			/* 遠距離では噛みつきの優先度をさらに下げ、尻尾攻撃の優先度を少し下げ、火炎攻撃の優先度を大幅に上げる。Aggressive よりはトリッキーな数値。*/
			return { C::ATTACK_TABLE_MID_MAX, C::ATTACK_TABLE_FAR_MAX, 10, 70, 20 };
		}


		Table BossAIConfigFactory::CreateGreenDragonConfig(float distance, const BossTypeParameters& params)
		{
			/* グリーンドラゴンは火炎攻撃を使用しないため、距離に応じて噛みつきと尻尾攻撃の優先度が変化するテーブルを使用する。HP 割合や直前の攻撃は考慮しないが、TSV パラメータは考慮する。*/
			const int bite = params.m_preferredBiteChance; //! TSV パラメータから噛みつきの優先度を取得
			const int tail = params.m_preferredTailChance; //! TSV パラメータから尻尾攻撃の優先度を取得
			const int fire = 0;							   //! グリーンドラゴンは火炎攻撃を使用しないため、優先度は常に 0

			/* グリーンドラゴン専用の距離境界（30 / 60）。他性格とは意図的に異なる。*/
			using C = BossAIConfig;

			/* 距離に応じて攻撃の優先度を調整する。近距離では噛みつきと尻尾攻撃を優先し、遠距離では両方の優先度を下げる。*/
			if (distance < C::ATTACK_TABLE_NEAR_MAX)
				return { 0.0f, C::ATTACK_TABLE_NEAR_MAX, bite, tail, fire };

			/* 中距離では噛みつきの優先度を少し下げ、尻尾攻撃の優先度を少し下げる。*/
			if (distance < C::ATTACK_TABLE_MID_MAX)
				return { C::ATTACK_TABLE_NEAR_MAX, C::ATTACK_TABLE_MID_MAX, bite, tail, fire };

			/* 遠距離では噛みつきの優先度をさらに下げ、尻尾攻撃の優先度をさらに下げる。*/
			return { C::ATTACK_TABLE_MID_MAX, C::ATTACK_TABLE_FAR_MAX, bite, tail, fire };
		}
	}
}