#include "stdafx.h"
#include "BossMoveState.h"
#include "Src/Actor/Character/Boss/AI/BossAIConfig.h"
#include "Src/Actor/Character/Boss/Status/BossManager.h"
#include <cstdlib>

namespace
{
    const float MOVE_TIME_DEFAULT = 1.5f;
    const float MOVE_TIME_DASH = 0.8f;
    const float MOVE_TIME_SIDESTEP = 1.0f;

    const float GREEN_DRAGON_STAGE_MIN_X = -50.0f; // 70 → プレイヤー側まで広げる
    const float GREEN_DRAGON_STAGE_MAX_X = 220.0f;
    const float GREEN_DRAGON_EDGE_MARGIN = 25.0f;
    const float GREEN_DRAGON_STRAFE_RANGE = 55.0f; // この距離以内は横 Walk 優先

    const float RED_DRAGON_TOO_CLOSE = 70.0f;
    const float RED_DRAGON_ATTACK_MIN = 70.0f;
    const float RED_DRAGON_IDEAL_MAX = 130.0f;
    const float RED_DRAGON_RETREAT_MUL = 1.40f;
}

namespace nsApp
{
    namespace nsState
    {
        void BossMoveState::Enter()
        {
            m_boss = static_cast<nsActor::Boss *>(m_owner);

            const auto &params = nsAI::BossTypeManager::GetBossTypeParameters(m_boss->GetBossType());

            m_isDashing = false;
            m_isSideStep = false;
            m_isRetreat = false;
            m_sideDir = (rand() % 2 == 0) ? 1.0f : -1.0f;

            switch (params.personality)
            {
            case nsAI::BossPersonality::Aggressive:
                m_isDashing = true;
                m_timer = MOVE_TIME_DASH;
                break;

            case nsAI::BossPersonality::Tricky:
            {
                m_timer = params.m_moveDuration;
                if (m_boss->GetBossType() == CharacterModelType::GreenDragon && m_boss->GetTarget())
                {
                    const float bossX = m_boss->GetPosition().x;
                    const float targetX = m_boss->GetTarget()->GetPosition().x;
                    const float distance = m_boss->GetDistanceToTarget();
                    // 戦闘距離内はほぼ横 Walk（挟み込み）
                    if (distance <= GREEN_DRAGON_STRAFE_RANGE)
                        m_isSideStep = true;
                    else
                        m_isSideStep = ((rand() % 100) < static_cast<int>(params.m_sideStepChance * 100.0f));
                    // 左右ランダム（前進と同じ向きにしない）
                    m_sideDir = (rand() % 2 == 0) ? 1.0f : -1.0f;
                    // 端だけ方向を制限
                    if (bossX <= GREEN_DRAGON_STAGE_MIN_X + GREEN_DRAGON_EDGE_MARGIN)
                        m_sideDir = 1.0f; // 左端 → 右へ
                    else if (bossX >= GREEN_DRAGON_STAGE_MAX_X - GREEN_DRAGON_EDGE_MARGIN)
                        m_sideDir = -1.0f; // 右端 → 左へ
                }
                else
                {
                    m_isSideStep = ((rand() % 100) < static_cast<int>(params.m_sideStepChance * 100.0f));
                }
                break;
            }

            case nsAI::BossPersonality::Defensive:
            case nsAI::BossPersonality::Balanced:
            default:
                if (m_boss->GetBossType() == CharacterModelType::RedDragon)
                {
                    const float distance = m_boss->GetDistanceToTarget();

                    if (distance < RED_DRAGON_TOO_CLOSE)
                    {
                        m_isRetreat = true;
                    }
                    else if (distance <= RED_DRAGON_IDEAL_MAX)
                    {
                        m_isSideStep = true;

                        if (m_boss->GetTarget())
                        {
                            const float dx = m_boss->GetTarget()->GetPosition().x - m_boss->GetPosition().x;
                            m_sideDir = (dx >= 0.0f) ? -1.0f : 1.0f;
                        }
                    }

                    m_timer = params.m_moveDuration;
                }
                else
                {
                    m_timer = MOVE_TIME_DEFAULT;
                }
                break;
            }

            m_boss->PlayAnimation(nsActor::BossAnimationID::Walk);
        }

        void BossMoveState::Update()
        {
            float dt = g_gameTime->GetFrameDeltaTime();
            m_timer -= dt;

            const auto &params = nsAI::BossTypeManager::GetBossTypeParameters(m_boss->GetBossType());
            const bool isRedDragon = (m_boss->GetBossType() == CharacterModelType::RedDragon);

            Vector3 toTarget = Vector3::Zero;
            Vector3 moveDir = Vector3::Zero;

            if (m_boss->GetTarget())
            {
                toTarget = m_boss->GetTarget()->GetPosition() - m_boss->GetPosition();
                toTarget.y = 0.0f;
                if (toTarget.LengthSq() > FLT_EPSILON)
                    toTarget.Normalize();
            }

            if (isRedDragon && m_isRetreat)
            {
                moveDir = toTarget * 1.0f;
            }
            else if (m_isSideStep)
            {
                moveDir = Vector3(m_sideDir, 0.0f, 0.0f);
            }
            else
            {
                moveDir = toTarget;
            }

            float speed = params.m_moveSpeed * (m_isDashing ? params.m_dashSpeedMultiplier : 1.0f);
            if (isRedDragon && m_isRetreat)
                speed *= RED_DRAGON_RETREAT_MUL;

            if (m_boss->GetBossType() == CharacterModelType::GreenDragon)
            {
                const float bossX = m_boss->GetPosition().x;
                const float distance = m_boss->GetDistanceToTarget();

                if (bossX <= GREEN_DRAGON_STAGE_MIN_X + GREEN_DRAGON_EDGE_MARGIN)
                {
                    m_isSideStep = true;
                    m_sideDir = 1.0f;
                    moveDir = Vector3(1.0f, 0.0f, 0.0f);
                }
                else if (bossX >= GREEN_DRAGON_STAGE_MAX_X - GREEN_DRAGON_EDGE_MARGIN)
                {
                    m_isSideStep = true;
                    m_sideDir = -1.0f;
                    moveDir = Vector3(-1.0f, 0.0f, 0.0f);
                }
                else if (distance <= GREEN_DRAGON_STRAFE_RANGE)
                {
                    m_isSideStep = true;
                    moveDir = Vector3(m_sideDir, 0.0f, 0.0f);
                }
                else if (m_isSideStep)
                {
                    moveDir = Vector3(m_sideDir, 0.0f, 0.0f);
                }
            }

            if (m_isSideStep && moveDir.LengthSq() > FLT_EPSILON)
            {
                m_boss->SetForward(moveDir);
                m_boss->PlayAnimation(nsActor::BossAnimationID::Walk);
            }
            else if (toTarget.LengthSq() > FLT_EPSILON)
            {
                m_boss->SetForward(toTarget);
            }

            Vector3 moveStep = moveDir * speed;
            moveStep.z = 0.0f;

            m_boss->GetController().Execute(moveStep, dt);

            if (isRedDragon && m_boss->GetTarget())
            {
                const float distance = m_boss->GetDistanceToTarget();

                if (distance < RED_DRAGON_TOO_CLOSE)
                {
                    m_isRetreat = true;
                    m_isSideStep = false;
                }
                else if (distance <= RED_DRAGON_IDEAL_MAX)
                {
                    m_isRetreat = false;
                    m_isSideStep = true;
                }
                else
                {
                    m_isRetreat = false;
                    m_isSideStep = false;
                }
            }

            uint8_t nextID = 0;
            if (RequestID(nextID))
                m_timer = 0.0f;
        }
        bool BossMoveState::RequestID(uint8_t &id)
        {
            const auto &params = nsAI::BossTypeManager::GetBossTypeParameters(m_boss->GetBossType());
            const float distance = m_boss->GetDistanceToTarget();

            if (m_boss->GetBossType() == CharacterModelType::RedDragon)
            {
                if (distance < RED_DRAGON_TOO_CLOSE)
                {
                    if (m_timer <= 0.0f)
                    {
                        id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
                        return true;
                    }
                    return false;
                }

                if (distance >= RED_DRAGON_ATTACK_MIN && distance <= RED_DRAGON_IDEAL_MAX)
                {
                    id = static_cast<uint8_t>(nsActor::BossStateID::enAttack);
                    return true;
                }

                if (m_timer <= 0.0f)
                {
                    id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
                    return true;
                }

                return false;
            }

            if (distance < params.m_moveStopDistance)
            {
                id = static_cast<uint8_t>(nsActor::BossStateID::enAttack);
                return true;
            }

            if (m_isDashing && distance >= 55.0f && m_timer <= 0.3f)
            {
                id = static_cast<uint8_t>(nsActor::BossStateID::enAttack);
                return true;
            }

            if (m_timer <= 0.0f)
            {
                id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
                return true;
            }

            return false;
        }
    }
}