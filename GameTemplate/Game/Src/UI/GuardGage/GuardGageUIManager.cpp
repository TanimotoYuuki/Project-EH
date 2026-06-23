#include "stdafx.h"
#include "GuardGageUIManager.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsUI
	{
		void GuardGaugeUIManager::Init()
		{
			m_playerCount = 0;

			for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
			{
				m_players[i] = nullptr;
				m_gaugeUI[i].Init();
			}
		}

		void GuardGaugeUIManager::Update()
		{
			for (int i = 0; i < m_playerCount; ++i)
				m_gaugeUI[i].Update();
		}

		void GuardGaugeUIManager::Render(RenderContext& rc)
		{
			for (int i = 0; i < m_playerCount; ++i)
				m_gaugeUI[i].Render(rc);
		}

		void GuardGaugeUIManager::RegisterPlayer(nsActor::Player* player)
		{
			if (player == nullptr)
				return;

			if (m_playerCount >= MAX_PLAYER_COUNT)
				return;

			m_players[m_playerCount] = player;
			m_gaugeUI[m_playerCount].SetTarget(player);
			++m_playerCount;
		}

		void GuardGaugeUIManager::ClearPlayers()
		{
			for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
			{
				m_players[i] = nullptr;
				m_gaugeUI[i].ClearTarget();
			}

			m_playerCount = 0;
		}
	}
}