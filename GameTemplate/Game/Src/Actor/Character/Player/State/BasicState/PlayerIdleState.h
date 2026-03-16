#pragma once
/**
* @file   PlayerIdleState.h
* @brief  PlayerのIdle状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/15
*/


#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerIdleState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerIdleState() = default;
			virtual ~PlayerIdleState() = default;


		public:
			/* ステートに入った際、1度呼ばれる。*/
			void Enter() override;
			/* ステート中、毎フレーム更新する。。*/
			void Update() override;
			/* ステートを抜ける際、呼ぶ。*/ 
			void Exit() override;
			/* ステートを遷移させる*/
			bool RequestID(uint8_t& id) override;

		private:
			nsActor::Player* m_player; /* プレイヤーの実態へのポインタ。*/
		};
	}
}
