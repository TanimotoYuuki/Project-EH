#include "stdafx.h"
#include "BossAttackState.h"
#include "Boss.h"
#include <random>

/*各攻撃ストラテジー。*/
#include "BiteAttackStrategy.h"
#include "TailAttackStrategy.h"
#include "FireAttackStrategy.h"

namespace nsApp
{
	namespace nsState
	{
		void BossAttackState::Enter()
		{
			m_boss = static_cast<nsActor::Boss*>(m_owner);


			/*ランダムで攻撃選択。*/
			static std::mt19937 gen(std::random_device{}());
			std::uniform_int_distribution<> dist(0, 99);
			int r = dist(gen);

			if (r<40)
			{
				m_strategy = std::make_unique<nsAI::BiteAttackStrategy>();
			}
			else if (r <70)
			{
				m_strategy = std::make_unique<nsAI::TailAttackStrategy>();
			}
			else
			{
				m_strategy = std::make_unique<nsAI::FireAttackStrategy>();
			}

			/*攻撃開始。*/
			if (m_strategy)
			{
				m_strategy->Enter(m_boss);
			}
		}

		void BossAttackState::Update()
		{
			if (m_strategy)
			{
				m_strategy->Update(m_boss);
			}
		}

		void BossAttackState::Exit()
		{
			/*念のため破棄。*/
			m_strategy.reset();
		}

		bool BossAttackState::RequestID(uint8_t& id)
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