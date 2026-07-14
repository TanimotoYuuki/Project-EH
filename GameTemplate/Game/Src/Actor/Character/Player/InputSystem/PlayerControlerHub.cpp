#include "stdafx.h"
#include "PlayerControlerHub.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/InputSystem/GamePadInputAdapter.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/Status/NPCStatusParameterTable.h"

namespace
{
	const auto PLAYER_ATTACK_DAMAGE_RATE = 1.0f;	  //! 操作中キャラクターの攻撃補正。
	const auto NPC_ATTACK_DAMAGE_RATE = 0.4f;		  //! NPC操作キャラクターの攻撃補正。
	const auto ATTACK_INTERVAL_STAGGER_PER_INDEX = 3; //! 0,3,6,9 フレームずらす
}

namespace nsApp
{
	void PlayerControlerHub::Initialize(const vector<nsActor::Player *> &players, const vector<PlayerSpawnData> &spawnDataList)
	{
		for (auto *player : m_players)
		{
			if (player != nullptr)
			{
				player->GetInputClass().SetInputDevice(nullptr);
				auto *brain = player->GetBrain();
				if (brain != nullptr)
				{
					brain->SetVirtualInputAdapter(nullptr);
				}
			}
		}

		/* プレイヤーとアダプターの初期化。*/
		m_players = players;
		m_gamePad = make_unique<GamePadInputAdapter>(0);
		m_adapters.clear();

		for (int i = 0; i < m_players.size(); ++i)
		{
			/* 各プレイヤーに専用の仮想入力アダプターを渡す。*/
			m_adapters.push_back(std::make_unique<VirtualInputAdapter>());

			/* NPC操作キャラクターの攻撃補正と攻撃間隔を設定。*/
			auto *brain = m_players[i]->GetBrain();
			if (brain != nullptr)
			{
				/* NPC操作キャラクターの攻撃補正を設定。*/
				auto *vAdapter = static_cast<VirtualInputAdapter *>(m_adapters[i].get());
				brain->SetVirtualInputAdapter(vAdapter);
				brain->SetPartyIndex(i);
			}

			/* 1P操作に割り当てるプレイヤーのインデックスを取得。*/
			if (spawnDataList[i].controllerType == ControllerType::Player_1P)
				m_target1PIndex = i;
		}

		/* 初期状態では1P操作キャラクターを操作する。*/
		SwitchActivePlayers(m_target1PIndex);
	}

	void PlayerControlerHub::Update()
	{
		/* 全Adapterのフレーム開始処理。*/
		for (auto &allAdapter : m_adapters)
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
		for (auto &adapter : m_adapters)
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
				m_players[i]->SetNpcControlled(!isActivePlayer);
			}
			else
			{
				/* NPCの入力デバイスを仮想入力アダプターに設定。*/
				m_players[i]->GetInputClass().SetInputDevice(m_adapters[i].get());
				m_players[i]->SetAttackDamageRate(NPC_ATTACK_DAMAGE_RATE);
				m_players[i]->SetNpcControlled(!isActivePlayer);

				/* NPCの攻撃間隔を設定。*/
				auto *brain = m_players[i]->GetBrain();
				if (brain != nullptr)
					brain->SetAttackInterval(CalcNpcAttackInterval(m_players[i], i));
			}
		}

		/* インデックスを更新。*/
		m_activePlayerIndex = targetIndex;
	}

	int PlayerControlerHub::CalcNpcAttackInterval(nsActor::Player *player, int partyIndex)
	{
		/* プレイヤーがnullptrの場合はデフォルトの攻撃間隔を返す。*/
		if (player == nullptr)
			return 60;

		/* 攻撃間隔の計算。*/
		const NPCStatusParameter &param = NPCStatusParameterTable::GetParameter(player->GetCurrentWeapon());

		/* 攻撃間隔を計算して返す。*/
		return param.attackInterval + partyIndex * ATTACK_INTERVAL_STAGGER_PER_INDEX;
	}
}