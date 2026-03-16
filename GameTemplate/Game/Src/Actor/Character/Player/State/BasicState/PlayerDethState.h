#pragma once
/**
* @file   PlayerDethState.h
* @brief  プレイヤーの死亡状態を管理するクラス。
* @author Yamaguchi Hayato。
* @date   2026/03/16
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerDethState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerDethState() = default;
			virtual ~PlayerDethState() = default;


		public:
		    /* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			/* ステートを遷移させる。*/
			bool RequestID(uint8_t& id) override;


		private:
			nsActor::Player* m_player; /* プレイヤーの実態へのポインタ。*/


		private:
			Vector3 m_fallSpeed;       /* 落下速度。*/
			
		};
	}
}
