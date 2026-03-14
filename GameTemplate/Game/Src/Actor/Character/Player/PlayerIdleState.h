#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/IState.h"
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


		private:
			nsActor::Player* m_player; /* プレイヤーの実態へのポインタ。*/
		};
	}
}
