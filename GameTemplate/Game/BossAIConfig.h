#pragma once

namespace nsApp
{
	namespace nsAI
	{
		struct BossAIConfig
		{
			static constexpr float CLOSE_ATTACK_DISTANCE = 35.0f;
			static constexpr float MEDIUM_ATTACK_DISTANCE = 90.0f;
			static constexpr float FAR_DISTANCE = 150.0f;

			static constexpr float MOVE_STOP_DISTANCE = 50.0f;

			static constexpr float IDLE_TIME = 2.0f;

			static constexpr float MOVE_DURATION = 3.0f;

			static constexpr float HIGH_HP_RATIO = 0.7f;
			static constexpr float LOW_HP_RATIO = 0.3f;

			static constexpr int FLINCH_DAMAGE_THRESHOLD = 200; // 怯むのに必要なダメージ。
			static constexpr float DAMAGE_RESET_TIME = 8.0f;	// 累計値がリセットされる時間。
			static constexpr float FLINCH_COOLDOWN = 0.5f;		// 一度怯んだら次怯むまでの猶予。

			struct AttackDistanceTable
			{
				float minDistance;
				float maxDistance;
				int biteChance;
				int tailChance;
				int fireChance;
			};

			static inline AttackDistanceTable GetAttackProbability(
				float distance,
				float hpRatio,
				bool lastAttackWasBite)
			{
				if (distance < 25.0f)
				{
					if (hpRatio > HIGH_HP_RATIO)
					{
						return {0.0f, 25.0f, lastAttackWasBite ? 50 : 70, 20, 10};
					}
					else if (hpRatio < LOW_HP_RATIO)
					{
						return {0.0f, 25.0f, lastAttackWasBite ? 40 : 50, 30, 20};
					}
					else
					{
						return {0.0f, 25.0f, lastAttackWasBite ? 50 : 65, 25, 10};
					}
				}
				else if (distance < 55.0f)
				{
					if (hpRatio > HIGH_HP_RATIO)
					{
						return {25.0f, 55.0f, 25, lastAttackWasBite ? 40 : 60, 15};
					}
					else if (hpRatio < LOW_HP_RATIO)
					{
						return {25.0f, 55.0f, 20, lastAttackWasBite ? 35 : 50, 30};
					}
					else
					{
						return {25.0f, 55.0f, 20, lastAttackWasBite ? 40 : 55, 25};
					}
				}
				else
				{
					if (hpRatio > HIGH_HP_RATIO)
					{
						return {55.0f, 1000.0f, 10, 15, lastAttackWasBite ? 70 : 75};
					}
					else if (hpRatio < LOW_HP_RATIO)
					{
						return {55.0f, 1000.0f, 20, 30, lastAttackWasBite ? 45 : 50};
					}
					else
					{
						return {55.0f, 1000.0f, 15, 20, lastAttackWasBite ? 65 : 70};
					}
				}
			}

			static constexpr float ROAR_HP_THRESHOLD_FIRST = 0.6f;
			static constexpr float ROAR_HP_THRESHOLD_SECOND = 0.3f;
			static constexpr float ROAR_COOLDOWN = 15.0f;

			static constexpr int ROAR_MOVE_CHANCE = 65;
			static constexpr int ROAR_ATTACK_CHANCE = 35;
		};
	}
}
