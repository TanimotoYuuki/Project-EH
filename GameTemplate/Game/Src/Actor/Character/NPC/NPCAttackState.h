#pragma once
/**
* @file   NPCAttackState.h
* @brief  NPCの攻撃状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/04/28
*/

#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Common/IState.h"

namespace nsApp
{
	namespace nsState
	{
		class NPCAttackState : public IState<NPCBrain>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			NPCAttackState() = default;
			virtual ~NPCAttackState() = default;


		public:
		    /* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override ;
			bool RequestID(uint8_t& id) override { return false; };


		private:
			NPCBrain* m_brain = nullptr; //! NPCの親クラスのポインタ。


		private:
			int m_attackTimer = 0;       //! 攻撃のタイマー。
		};

	}
}
