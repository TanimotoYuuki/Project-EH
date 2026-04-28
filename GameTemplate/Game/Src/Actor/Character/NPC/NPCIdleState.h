#pragma once
/**
* @file NPCIdleState.h
* @brief NPCの待機状態を管理するクラス。
* @author Yamaguchi Hayato
* @date 2026/04/27
*/

#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Common/IState.h"

namespace nsApp
{
	namespace nsState
	{
		class NPCIdleState :public IState<NPCBrain>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			NPCIdleState() = default;
			virtual ~NPCIdleState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override { return false; };



		private:
			NPCBrain* m_brain = nullptr; //! NPCの親クラスのポインタ。
		};
	}
}
