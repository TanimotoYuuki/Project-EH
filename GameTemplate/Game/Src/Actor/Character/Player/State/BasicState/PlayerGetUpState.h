#pragma once
/**
* @file PlayerGetUpState.h
* @brief プレイヤーの起き上がり状態を管理するクラス。
* @author Yamaguchi Hayato
* @date 2026/03/23
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerGetUpState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerGetUpState() = default;
			virtual ~PlayerGetUpState() = default;


		public:
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override { return false; };


		private:
			nsActor::Player* m_player = nullptr;
		};


	}
}

