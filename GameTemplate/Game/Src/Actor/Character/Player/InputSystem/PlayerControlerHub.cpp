#include "stdafx.h"
#include "PlayerControlerHub.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/InputSystem/GamePadInputAdapter.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/Status/NPCStatusParameterTable.h"

namespace
{
	const auto PLAYER_ATTACK_DAMAGE_RATE = 1.0f; //! 操作中キャラクターの攻撃補正。
	const auto NPC_ATTACK_DAMAGE_RATE = 0.4f;    //! NPC操作キャラクターの攻撃補正。
	const auto NPC_ATTACK_INTERVAL = 60;         //! NPCの攻撃間隔。
}

namespace nsApp
{
	void PlayerControlerHub::Initialize(const vector<nsActor::Player*>& players, const vector<PlayerSpawnData>& spawnDataList)
	{
		/* プレイヤーとアダプターの初期化。*/
		m_players = players;
		m_gamePad = make_unique<GamePadInputAdapter>(0);
		m_adapters.clear();

		for (int i = 0; i < m_players.size(); ++i)
		{
			/* 各プレイヤーに専用の仮想入力アダプターを渡す。*/ 
			m_adapters.push_back(std::make_unique<VirtualInputAdapter>());

			/* NPC操作キャラクターの攻撃補正と攻撃間隔を設定。*/
			auto* brain = m_players[i]->GetBrain();
			if (brain != nullptr)
			{
				/* NPC操作キャラクターの攻撃補正を設定。*/
				auto* vAdapter = static_cast<VirtualInputAdapter*>(m_adapters[i].get());
				brain->SetVirtualInputAdapter(vAdapter);
			}

			/* 1P操作に割り当てるプレイヤーのインデックスを取得。*/
			if (spawnDataList[i].controllerType == ControllerType::Player_1P)
				m_target1PIndex = i;
		}

		//////////////////////////////////////////////////////////////////
		// ゲーム開始時は強制的に n番目 をプレイヤー操作にする
		// （※キャラ選択画面ができたら、ここで「選ばれた番号」を渡すようにします）
		SwitchActivePlayers(m_target1PIndex);
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
		if (m_gamePad)
			m_gamePad->Reset();
		for (auto& adapter : m_adapters)
			adapter->Reset();


		for (int i = 0; i < m_players.size(); ++i)
		{
			/* 操作キャラとNPCを切り替える。*/
			const bool isActivePlayer = (i == targetIndex);

			/* 入力デバイスを切り替える。*/
			if (isActivePlayer)
			{
				/* 操作キャラの入力デバイスをゲームパッドに設定。*/
				m_players[i]->GetInputClass().SetInputDevice(m_gamePad.get());
				m_players[i]->SetAttackDamageRate(PLAYER_ATTACK_DAMAGE_RATE);
			}
			else
			{
				/* NPCの入力デバイスを仮想入力アダプターに設定。*/
				m_players[i]->GetInputClass().SetInputDevice(m_adapters[i].get());
				m_players[i]->SetAttackDamageRate(NPC_ATTACK_DAMAGE_RATE);

				/* NPCの攻撃間隔を設定。*/
				auto* brain = m_players[i]->GetBrain();
				if (brain != nullptr)
					brain->SetAttackInterval(NPC_ATTACK_INTERVAL);
			}
		}

		/* インデックスを更新。*/
		m_activePlayerIndex = targetIndex;
	}
}