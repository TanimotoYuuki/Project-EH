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
                switch (params.personality)
                {
                case BossPersonality::Aggressive:
                    return CreateAggressiveConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                case BossPersonality::Balanced:
                    return CreateBalancedConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                case BossPersonality::Defensive:
                    return CreateDefensiveConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                case BossPersonality::Tricky:
                    return CreateTrickyConfig(currentDistance, hpRatio, lastAttackWasBite, params);
                default:
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
                    int bite = params.m_preferredBiteChance;
                    int tail = params.m_preferredTailChance;
                    int fire = params.m_preferredFireChance;

                    if (lastAttackWasBite)
                    {
                        bite -= 30;
                        tail += 30;
                    }
                    return {0.0f, 25.0f, bite, tail, fire};
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
                    return {0.0f, 25.0f, lastAttackWasBite ? 20 : 45, 35, 20};
                }
                else if (distance < 55.0f)
                {
                    return {25.0f, 55.0f, 25, 40, 35};
                }
                else
                {
                    return {55.0f, 1000.0f, lastAttackWasBite ? 20 : 45, 35, 20};
                }
            }

            /*防御的。*/
            static BossAIConfig::AttackDistanceTable CreateDefensiveConfig(
                float distance, float hpRatio, bool lastAttackWasBite, const BossTypeParameters &params)
            {
                if (distance < 25.0f)
                {
                    int bite = lastAttackWasBite ? 25 : params.m_preferredBiteChance;
                    int tail = lastAttackWasBite ? 55 : params.m_preferredTailChance;
                    return {0.0f, 25.0f, bite, tail, params.m_preferredFireChance};
                }
                else if (distance < 55.0f)
                {
                    return {25.0f, 55.0f, 20, 50, 30};
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
                    return {0.0f, 25.0f, 15, 70, 15};
                }
                else if (distance < 55.0f)
                {
                    return {25.0f, 55.0f, 15, 70, 15};
                }
                else
                {
                    return {55.0f, 1000.0f, params.m_preferredBiteChance, params.m_preferredTailChance, params.m_preferredFireChance};
                }
            }
        };
    }
}