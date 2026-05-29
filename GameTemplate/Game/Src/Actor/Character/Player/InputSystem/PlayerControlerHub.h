#pragma once
/*
 * @file    PlayerControlerHub.h
 * @brief   プレイヤーの入力を管理するクラス。
 * @author  Yamaguchi Hayato
 * @detail 
 */

#include "Src/Actor/Character/Player/Component/PlayerSpawnData.h"
#include "Src/Actor/Character/Player/InputSystem/IInputDevice.h"
#include "Src/Actor/Character/Player/InputSystem/GamePadInputAdapter.h"
#include <vector>
#include <memory>
#include <map>

namespace nsApp
{
	/* ※今回 std の使用箇所が多いので止む終えず使用。*/
	using namespace std;


	namespace nsActor {
		class Player;
	}
	class GamePadAdapter;
	class PlayerControlerHub
	{
	public:
		/** 
		 * @brief 初期化。
		 * @param players: プレイヤーのリスト。
		 * @details プレイヤーのリストを受け取り、各プレイヤーに対応する入力デバイスを初期化します。
		 */
		void Initialize(const vector<nsActor::Player*>& players, const vector<PlayerSpawnData>& spawnDataList);

		/**
		 * @brief 更新処理。
		 */
		void Update();

		/**
		 * @brief 操作キャラを切り替える。
		 * @param targetIndex: 切り替えるプレイヤーのインデックス。
		 */
		void SwitchActivePlayers(int targetIndex);


	private:
		unique_ptr<GamePadInputAdapter> m_gamePad;		 //! ゲームパッドアダプターを管理する。
		vector<unique_ptr<IInputDevice>> m_adapters;	 //! 全Adapterを管理する。
		vector<nsActor::Player*> m_players;				 //! プレイヤーのリスト。

		int m_activePlayerIndex = 0;					 //! 現在操作しているプレイヤーのインデックス。
		int m_nextPadIndex = 0;							 //! 次に割り当てるゲームパッドのインデックス。
	};
}
