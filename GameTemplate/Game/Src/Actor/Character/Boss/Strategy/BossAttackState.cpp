#include "stdafx.h"
#include "BossAttackState.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Boss/AI/BossAIConfigFactory.h"
#include "Src/Actor/Character/Boss/Status/BossManager.h"
#include <memory>
#include <random>

#include "BiteAttackStrategy.h"
#include "TailAttackStrategy.h"
#include "FireAttackStrategy.h"
#include "ChargeAttackStrategy.h"

namespace
{
    enum AttackType : uint8_t
    {
        enBite = 0,
        enTail = 1,
        enFire = 2,
        enCharge = 3,
    };

    std::mt19937 g_randomGen(std::random_device{}());

    std::unique_ptr<nsApp::nsAI::IBossAttackStrategy> MakeStrategy( nsApp::nsActor::Boss* boss, uint8_t attackType)
    {
        switch (attackType)
        {
        case enBite:
            boss->RecordLastAttackType(enBite);
            return std::make_unique<nsApp::nsAI::BiteAttackStrategy>();

        case enTail:
            boss->RecordLastAttackType(enTail);
            return std::make_unique<nsApp::nsAI::TailAttackStrategy>();

        case enFire:
        default:
            boss->RecordLastAttackType(enFire);
            return std::make_unique<nsApp::nsAI::FireAttackStrategy>();
        }
    }

    uint8_t PickAttackType(int roll, const nsApp::nsAI::BossAIConfig::AttackDistanceTable& table)
    {
        if (roll < table.biteChance)
            return enBite;

        if (roll < table.biteChance + table.tailChance)
            return enTail;

        return enFire;
    }

    std::unique_ptr<nsApp::nsAI::IBossAttackStrategy> SelectAttackStrategy(nsApp::nsActor::Boss* boss)
    {
        float    distance = boss->GetDistanceToTarget();
        float    hpRatio = boss->GetHPRatio();
        uint8_t  lastAttackType = boss->GetLastAttackType();
        bool     lastAttackWasBite = (lastAttackType == enBite);

        const auto& params = nsApp::nsAI::BossTypeManager::GetBossTypeParameters(boss->GetBossType());

        if (params.personality == nsApp::nsAI::BossPersonality::Aggressive && distance >= 20.0f)
        {
            std::uniform_int_distribution<> chargeDist(0, 99);
            if (chargeDist(g_randomGen) < 50)
            {
                boss->RecordLastAttackType(enCharge);
                return std::make_unique<nsApp::nsAI::ChargeAttackStrategy>();
            }
        }

        /* 情報を取得。*/
        auto table = nsApp::nsAI::BossAIConfigFactory::CreateConfigForBoss( boss->GetBossType(), distance, hpRatio,lastAttackWasBite);

		/* 攻撃抽選。*/
        std::uniform_int_distribution<> dist(0, 99);

        //! TutorialBoss：直前と同じ攻撃は避ける（最大 3 回）。
        for (int attempt = 0; attempt < 3; ++attempt)
        {
            int r = dist(g_randomGen);
            uint8_t pickedType = PickAttackType(r, table);

            if (boss->GetBossType() != nsApp::CharacterModelType::TutorialBoss || pickedType != lastAttackType || attempt == 2)
                return MakeStrategy(boss, pickedType);
        }

        return MakeStrategy(boss, enFire);
    }
}

namespace nsApp
{
    namespace nsState
    {
        void BossAttackState::Enter()
        {
            m_boss = static_cast<nsActor::Boss*>(m_owner);
            m_strategy = SelectAttackStrategy(m_boss);

            if (m_strategy)
                m_strategy->Enter(m_boss);
        }

        void BossAttackState::Update()
        {
            if (m_strategy && m_boss)
                m_strategy->Update(m_boss);
        }

        void BossAttackState::Exit()
        {
            if (m_strategy && m_boss)
                m_strategy->Exit(m_boss);

            m_strategy.reset();
            m_boss = nullptr;
        }

        bool BossAttackState::RequestID(uint8_t& id)
        {
            if (m_strategy && m_strategy->IsEnd())
            {
                if (m_boss && m_boss->GetBossType() == CharacterModelType::RedDragon && m_boss->GetDistanceToTarget() < 70.0f)
                    id = static_cast<uint8_t>(nsActor::BossStateID::enMove);

                else
                    id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);

                return true;
            }
            return false;
        }
    }
}