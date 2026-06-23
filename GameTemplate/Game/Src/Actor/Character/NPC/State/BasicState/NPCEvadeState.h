#pragma once

/**
 * @file   NPCEvadeState.h
 * @brief  NPC が攻撃オブジェクトを避けて走る状態。
 * @author Yamaguchi Hayato
 * @date   2026/05/01
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"

namespace nsApp
{
	namespace nsState
	{
		/**
		 * @class NPCEvadeState
		 * @brief 危険時に回避移動する。判定・経路は Brain に委譲する。
		 */
		class NPCEvadeState : public IState<NPCBrain>
		{
		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id) override { return false; }


		private:
			/**
			 * @brief 回避移動の入力をクリアする処理。
			 */
			void ClearMoveInput();

			/**
			 * @brief 回避移動の方向を決定する処理。
			 */
			void MoveEvadeDirection();

			/**
			 * @brief ロック中の回避方向をリセットする処理。
			 */
			void ResetLockedDirection();


		private:
			NPCBrain* m_brain = nullptr;				 //! NPCBrain のポインタ。
			nsActor::Player* m_body = nullptr;			 //! NPC の Player クラスのポインタ。
			VirtualInputAdapter* m_vInput = nullptr;     //! VirtualInputAdapter のポインタ。

			Vector3 m_lockedDirection = Vector3::Zero;   //! ロック中の回避方向。

			int m_directionLockTimer = 0;				 //! 方向ロックの残りフレーム。

			bool m_hasLockedDirection = false;		     //! 有効なロック方向を保持しているか。
		};
	}
}