#pragma once

#include "BossTypeManager.h"
#include "BossAIConfig.h"

namespace nsApp
{
    namespace nsAI
    {
        class BossAIConfigFactory
        {
        public:
            static BossAIConfig::AttackDistanceTable CreateConfigForBoss(
                CharacterModelType bossType,
                float currentDistance,
                float hpRatio,
                bool lastAttackWasBite)
            {
                const BossTypeParameters &params = BossTypeManager::GetBossTypeParameters(bossType);

                /*ボスのパーソナリティに基づいて確率を調整。*/
                if (params.personality == BossPersonality::Aggressive)
                {
                    return CreateAggressiveConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                }
                else if (params.personality == BossPersonality::Balanced)
                {
                    return CreateBalancedConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                }
                else if (params.personality == BossPersonality::Defensive)
                {
                    return CreateDefensiveConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                }
                else if (params.personality == BossPersonality::Tricky)
                {
                    return CreateTrickyConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                }
                else
                {
                    return CreateBalancedConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                }
            }

        private:
            /*攻撃的なボス。*/
            static BossAIConfig::AttackDistanceTable CreateAggressiveConfig(
                float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters &params)
            {
                if (distance < 25.0f)
                {
                    return {
                        0.0f, 25.0f,
                        lastAttackWasBite ? 50 : 80,
                        params.m_preferredTailChance,
                        params.m_preferredFireChance};
                }
                else if (distance < 55.0f)
                {
                    return {25.0f, 55.0f, 30, 50, 20};
                }
                else
                {
                    return {55.0f, 1000.0f, 10, 20, 70};
                }
            }

            /*バランス型。*/
            static BossAIConfig::AttackDistanceTable CreateBalancedConfig(
                float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters &params)
            {
                if (distance < 25.0f)
                {
                    return {0.0f, 25.0f, 40, 30, 30};
                }
                else if (distance < 55.0f)
                {
                    return {25.0f, 55.0f, 25, 40, 35};
                }
                else
                {
                    return {55.0f, 1000.0f, 15, 20, 65};
                }
            }

            /*防御的。*/
            static BossAIConfig::AttackDistanceTable CreateDefensiveConfig(
                float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters &params)
            {
                if (distance < 25.0f)
                {
                    return {0.0f, 25.0f, 50, 30, 20};
                }
                else if (distance < 55.0f)
                {
                    return {25.0f, 55.0f, 30, 40, 30};
                }
                else
                {
                    return {55.0f, 1000.0f, 20, 30, 50};
                }
            }

            /*トリッキー。*/
            static BossAIConfig::AttackDistanceTable CreateTrickyConfig(
                float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters &params)
            {
                if (distance < 25.0f)
                {
                    return {0.0f, 25.0f, 20, 60, 20};
                }
                else if (distance < 55.0f)
                {
                    return {25.0f, 55.0f, 15, 65, 20};
                }
                else
                {
                    return {55.0f, 1000.0f, 10, 70, 20};
                }
            }
        };
    }
}