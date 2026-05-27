#include "stdafx.h"
#include "BossAttackState.h"
#include "Boss.h"
#include "BossAIConfigFactory.h"
#include "BossTypeManager.h"
#include <memory>
#include <random>

/*各攻撃ストラテジー。*/
#include "BiteAttackStrategy.h"
#include "TailAttackStrategy.h"
#include "FireAttackStrategy.h"

namespace
{
	/*攻撃タイプ定数。*/
	enum AttackType : uint8_t
	{
		enBite = 0,
		enTail = 1,
		enFire = 2,
	};

	/*ランダム生成器。*/
	std::mt19937 g_randomGen(std::random_device{}());

	std::unique_ptr<nsApp::nsAI::IBossAttackStrategy> SelectAttackStrategy(nsApp::nsActor::Boss *boss)
	{
		float distance = boss->GetDistanceToTarget();
		float hpRatio = boss->GetHPRatio();
		uint8_t lastAttackType = boss->GetLastAttackType();

		/*前回の攻撃タイプから判定。*/
		bool lastAttackWasBite = (lastAttackType == enBite);

		/*ボスタイプに応じた攻撃確率テーブルを取得。*/
		auto table = nsApp::nsAI::BossAIConfigFactory::CreateConfigForBoss(
			boss->GetBossType(),
			distance,
			hpRatio,
			lastAttackWasBite);

		std::uniform_int_distribution<> dist(0, 99);
		int r = dist(g_randomGen);

		/*確率に基づいて攻撃を選択。*/
		if (r < table.biteChance)
		{
			boss->RecordLastAttackType(enBite);
			return std::make_unique<nsApp::nsAI::BiteAttackStrategy>();
		}
		else if (r < table.biteChance + table.tailChance)
		{
			boss->RecordLastAttackType(enTail);
			return std::make_unique<nsApp::nsAI::TailAttackStrategy>();
		}
		else
		{
			boss->RecordLastAttackType(enFire);
			return std::make_unique<nsApp::nsAI::FireAttackStrategy>();
		}
	}
}

namespace nsApp
{
	namespace nsState
	{
		void BossAttackState::Enter()
		{
			m_boss = static_cast<nsActor::Boss *>(m_owner);

			m_strategy = SelectAttackStrategy(m_boss);

			/*攻撃開始。*/
			if (m_strategy)
			{
				m_strategy->Enter(m_boss);
			}
		}

		void BossAttackState::Update()
		{
			if (m_strategy && m_boss)
			{
				m_strategy->Update(m_boss);
			}
		}

		void BossAttackState::Exit()
		{
			/*攻撃判定を無効化。*/
			if (m_strategy && m_boss)
			{
				m_strategy->Exit(m_boss);
			}
			m_strategy.reset();
			m_boss = nullptr;
		}

		bool BossAttackState::RequestID(uint8_t &id)
		{
			/*攻撃終了したらIdleへ。*/
			if (m_strategy && m_strategy->IsEnd())
			{
				id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
				return true;
			}
			return false;
		}
	}
}