#include "stdafx.h"
#include "ChargeAttackStrategy.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Boss/Status/BossManager.h"
#include <random>

namespace
{
    const float CHARGE_DURATION = 1.5f;   //! 突進の最大継続時間（秒）。
    const float CHARGE_SPEED_BASE = 150.0f; //! 突進の基本速度。
    const float CAMERA_EDGE_X = 140.0f; //! カメラ端のX座標（画面端ギリギリ）。
    const float ARRIVE_THRESHOLD = 8.0f;   //! 目標到達とみなす距離。

    const float TURN_DURATION = 1.2f; //! 方向転換にかける時間（秒）。
    const float TURN_WALK_SPEED = 5.0f; //! 方向転換中のゆっくり歩き速度。

    std::mt19937 g_rng(std::random_device{}()); //! 乱数生成器。
}

namespace nsApp
{
    namespace nsAI
    {
        void ChargeAttackStrategy::Enter(nsActor::Boss* boss)
        {
            m_timer = CHARGE_DURATION;
            m_turnTimer = TURN_DURATION;
            m_phase = Phase::Charge;

            if (boss->GetTarget())
            {
                Vector3 bossPos = boss->GetPosition();
                Vector3 targetPos = boss->GetTarget()->GetPosition();

                /*プレイヤーへの方向を計算。*/
                Vector3 toPlayer = targetPos - bossPos;
                toPlayer.y = 0.0f;
                toPlayer.z = 0.0f;
                if (toPlayer.LengthSq() > FLT_EPSILON)
                    toPlayer.Normalize();

                /*50%でプレイヤー方向、50%で逆方向に突進。*/
                std::uniform_int_distribution<int> coin(0, 1);
                const bool chargeToward = (coin(g_rng) == 0);
                m_chargeDir = chargeToward
                    ? toPlayer
                    : Vector3(-toPlayer.x, 0.0f, 0.0f);

                /*突進先をカメラ端に設定。*/
                const float edgeX = (m_chargeDir.x > 0.0f) ? CAMERA_EDGE_X : -CAMERA_EDGE_X;
                m_targetPosition = Vector3(edgeX, bossPos.y, 0.0f);

                boss->SetForward(m_chargeDir);

                boss->LockRotation(true);
            }

            /*突進速度を設定（ダッシュ倍率を適用）。*/
            const auto& params = nsAI::BossTypeManager::GetBossTypeParameters(boss->GetBossType());
            m_chargeSpeed = CHARGE_SPEED_BASE * params.m_dashSpeedMultiplier;

            boss->GetBiteHit().Enable();
            boss->GetBiteHit().Update(boss->GetPosition());
            boss->PlayAnimation(nsActor::BossAnimationID::BiteAttack);
        }


        void ChargeAttackStrategy::Update(nsActor::Boss* boss)
        {
            float dt = g_gameTime->GetFrameDeltaTime();

            if (m_phase == Phase::Charge)
            {
                m_timer -= dt;

                Vector3 bossPos = boss->GetPosition();
                Vector3 dir = m_targetPosition - bossPos;
                dir.y = 0.0f;
                dir.z = 0.0f;

                const bool arrived = dir.Length() < ARRIVE_THRESHOLD;
                const bool timedOut = m_timer <= 0.0f;

                if (!arrived && !timedOut)
                {
                    /*物理を無視してX座標を直接移動する（突進はすべてをすり抜ける）。*/
                    dir.Normalize();
                    boss->GetPosition().x += dir.x * m_chargeSpeed * dt;
                    boss->GetPosition().z = 0.0f;
                    boss->GetController().SetPosition(boss->GetPosition());
                    boss->GetBiteHit().Update(boss->GetPosition());
                }
                else
                {
                    /*突進終了 → 方向転換フェーズへ。*/
                    m_phase = Phase::TurnAround;
                    boss->GetBiteHit().Disable();
                    boss->PlayAnimation(nsActor::BossAnimationID::Walk);
                }
            }
            else
            {
                /*方向転換フェーズ：Walk アニメしながら逆方向（プレイヤー側）を向く。*/
                m_turnTimer -= dt;

                const float tClamped = max(0.0f, min(1.0f, 1.0f - (m_turnTimer / TURN_DURATION)));

                /*突進方向 → 逆方向 へ線形補間して向きを更新。*/
                Vector3 endDir = Vector3(-m_chargeDir.x, 0.0f, 0.0f);
                Vector3 newForward;
                newForward.x = m_chargeDir.x + (endDir.x - m_chargeDir.x) * tClamped;
                newForward.y = 0.0f;
                newForward.z = 0.0f;
                if (newForward.LengthSq() > FLT_EPSILON)
                    newForward.Normalize();
                boss->SetForward(newForward);

                /*ゆっくり逆方向へ歩く。*/
                Vector3 moveStep = endDir * TURN_WALK_SPEED;
                moveStep.z = 0.0f;
                boss->GetController().Execute(moveStep, dt);
            }
        }


        void ChargeAttackStrategy::Exit(nsActor::Boss* boss)
        {
            if (boss)
            {
                boss->GetBiteHit().Disable();
                boss->LockRotation(false);
            }
        }


        bool ChargeAttackStrategy::IsEnd()
        {
            return m_phase == Phase::TurnAround && m_turnTimer <= 0.0f;
        }
    }
}