#pragma once

/**
 * @file   NPCChaseState.h
 * @brief  NPCの追跡状態を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/04/27
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"

namespace nsApp
{
	namespace nsState
	{
		class NPCChaseState : public IState<NPCBrain>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			NPCChaseState() = default;
			virtual ~NPCChaseState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override { return false; };


		private:
			/* 
			 * @brief 味方を助ける行動。
			 * @param body: NPCの体。
			 * @param helpTarget: 助ける対象。
			 */
			bool ExecuteHelpAction(nsActor::Player* body, nsActor::Player* helpTarget);


			/**
			 * @brief 距離をつめる対象を設定する。
			 * @param body: NPCの体。
			 * @param target: 距離をつめる対象。
			 */
			void ExecuteChaseAction(nsActor::Player* body, nsActor::Sandbag* target);

		private:
			NPCBrain* m_brain = nullptr; //! NPCの親クラスのポインタ。


		private:
			Vector3 m_myPosition = Vector3::Zero;     //! NPCの現在位置。
			Vector3 m_targetPosition = Vector3::Zero; //! ターゲットの現在位置。
			Vector3 m_difference = Vector3::Zero;     //! NPCとターゲットの位置の差分。


			float m_distance = 0.0f;				  //! NPCとターゲットの距離。
		};
	}
}
