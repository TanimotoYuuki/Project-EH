#include "stdafx.h"
#include "Src/UI/ReboneGage/ReboneGageManager.h"

namespace nsApp
{
	namespace nsUI
	{
		void ReboneGaugeUIManager::Init()
		{
			m_players.clear();

			m_gaugeUI.Init();

			/* 仮位置。画面に出る位置へ調整してください。*/
			m_gaugeUI.SetPosition(Vector3(10.0f, 80.0f, 0.0f));
		}


		void ReboneGaugeUIManager::Update()
		{
			nsActor::Player* target = FindBeingHelpedPlayer();

			if (target != m_gaugeUI.GetTarget())
			{
				if (target != nullptr)
					m_gaugeUI.SetTarget(target);
				else
					m_gaugeUI.ClearTarget();
			}

			m_gaugeUI.Update();
		}


		void ReboneGaugeUIManager::Render(RenderContext& rc)
		{
			m_gaugeUI.Render(rc);
		}


		void ReboneGaugeUIManager::RegisterPlayer(nsActor::Player* player)
		{
			if (player == nullptr)
				return;

			m_players.push_back(player);
		}


		void ReboneGaugeUIManager::ClearPlayers()
		{
			m_players.clear();
			m_gaugeUI.ClearTarget();
		}


		nsActor::Player* ReboneGaugeUIManager::FindBeingHelpedPlayer() const
		{
			for (auto* player : m_players)
			{
				if (player == nullptr)
					continue;

				auto& rescue = player->GetRescueStatusManager();

				if (player->IsDeath() && rescue.IsBeingHelped())
					return player;
			}

			return nullptr;
		}
	}
}