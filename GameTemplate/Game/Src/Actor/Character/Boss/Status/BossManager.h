#pragma once
/**
 * @file   BossManager.h
 * @brief  ボスタイプ別パラメータの取得 API。
 */

#include "Src/Actor/Character/Common/CharacterModel.h"

namespace nsApp
{
	namespace nsAI
	{
		/**
		 * @enum BossPersonality
		 * @brief ボス AI の性格タイプ。
		 */
		enum class BossPersonality
		{
			Aggressive,
			Balanced,
			Defensive,
			Tricky
		};

		/**
		 * @struct BossTypeParameters
		 * @brief TSV / デフォルト表から読み込んだボス 1 種別分の AI パラメータ。
		 */
		struct BossTypeParameters
		{
			BossPersonality personality;

			float m_moveSpeed;
			float m_moveStopDistance;

			float m_idleTime;
			float m_moveDuration;
			float m_roarCooldown;

			int m_preferredBiteChance;
			int m_preferredTailChance;
			int m_preferredFireChance;

			int   m_baseHP;
			float m_Multiplier;

			bool hasSpecialAbility;
			bool canRoar;

			float m_dashSpeedMultiplier = 1.0f;
			float m_sideStepChance = 0.0f;
			bool  m_useAirReposition = false;
		};

		/**
		 * @class BossTypeManager
		 * @brief ボスタイプからパラメータを取得する静的マネージャ。
		 */
		class BossTypeManager
		{
		public:
			static const BossTypeParameters& GetBossTypeParameters(CharacterModelType bossType);
		};
	}
}
