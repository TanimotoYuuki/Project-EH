#include "stdafx.h"
#include "PlayerControlerHub.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/InputSystem/GamePadInputAdapter.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace nsApp
{
	void PlayerControlerHub::Initialize(const vector<nsActor::Player*>& players, const vector<PlayerSpawnData>& spawnDataList)
	{
		m_players = players;
		m_gamePad = make_unique<GamePadInputAdapter>(0);
		m_adapters.clear();

		int target1PIndex = -1;

		for (int i = 0; i < m_players.size(); ++i)
		{
			m_adapters.push_back(std::make_unique<VirtualInputAdapter>());

			// 脳みそ(Brain)には「君専用のAIパッド」を渡す。これで絶対安全！
			auto* brain = m_players[i]->GetBrain();
			if (brain != nullptr)
			{
				auto* vAdapter = static_cast<VirtualInputAdapter*>(m_adapters[i].get());
				brain->SetVirtualInputAdapter(vAdapter);
			}

			if (spawnDataList[i].controllerType == ControllerType::Player_1P)
				target1PIndex = i;
		}

		//////////////////////////////////////////////////////////////////
		// ゲーム開始時は強制的に n番目 をプレイヤー操作にする
		// （※キャラ選択画面ができたら、ここで「選ばれた番号」を渡すようにします）
		SwitchActivePlayers(target1PIndex);
		///////////////////////////////////////////////////////////////////
	}


	void PlayerControlerHub::Update()
	{
		/* 全Adapterのフレーム開始処理。*/
		for (auto& allAdapter : m_adapters)
			allAdapter->BeginFlame();

		/* 操作キャラ切り替え。*/
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			m_nextPadIndex = (m_activePlayerIndex + 1) % m_players.size();
			SwitchActivePlayers(m_nextPadIndex);
		}			
	}


	void PlayerControlerHub::SwitchActivePlayers(int targetIndex)
	{
		/* 範囲外チェック。*/
		if (targetIndex < -1 || targetIndex >= static_cast<int>(m_players.size()))
			return;

		/* リセット。*/
		if(m_gamePad)
			m_gamePad->Reset();
		for (auto& adapter : m_adapters)
			adapter->Reset();


		for (int i = 0; i < m_players.size(); ++i)
		{
			if(i == targetIndex)
				/* 接続先を切り替える。*/
				m_players[i]->GetInputClass().SetInputDevice(m_gamePad.get());

			else
				/* 新しい操作キャラを物理パッドに切り替え。*/
				m_players[i]->GetInputClass().SetInputDevice(m_adapters[i].get());
		}

		/* インデックスを更新。*/
		m_activePlayerIndex = targetIndex;
	}
}