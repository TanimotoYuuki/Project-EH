#pragma once

/**
 * @file   NPCChaseState.h
 * @brief  NPCの追跡状態を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/04/27
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	namespace nsState
	{
		class NPCChaseState : public IState<NPCBrain>
		{
		public:
			NPCChaseState() = default;
			virtual ~NPCChaseState() = default;

		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id) override { return false; };


		private:
			/**
			 * @brief 味方を助ける行動。
			 * @param helpTarget 助ける対象。
			 */
			bool ExecuteHelpAction(nsActor::Player* helpTarget);

			/**
			 * @brief 対象へ近づき、攻撃可能なら攻撃ステートへ遷移する。
			 * @param target 追跡対象。
			 */
			void ExecuteChaseAction(nsActor::ICharacter* target);

			/**
			 * @brief 攻撃ステートへ遷移する。
			 */
			void TransitionToAttackState();

			/**
			 * @brief 仮想入力の移動系入力を止める。
			 */
			void ClearMoveInput();

			/**
			 * @brief 現在の差分方向へ移動入力を入れる。
			 */
			void MoveTowardTarget();

			/**
			 * @brief 距離計算。
			 * @param targetObject 距離を計算する対象。
			 */
			inline void ComputeDistance(nsActor::ICharacter* targetObject)
			{
				m_difference = targetObject->GetPosition() - m_body->GetPosition();
				m_difference.y = 0.0f;
				m_distance = m_difference.Length();
			}

			/**
			 * @brief 武器の種類に応じて攻撃開始距離を返す。
			 * @param type 武器の種類。
			 * @return 攻撃開始距離。
			 */
			float CharacterToBeChosen(WeaponType type) const;
			

		private:
			NPCBrain* m_brain = nullptr;             //! NPCの親クラス。
			nsActor::Player* m_body = nullptr;       //! NPC本体。
			VirtualInputAdapter* m_vInput = nullptr; //! NPCの仮想入力。


		private:
			Vector3 m_myPosition = Vector3::Zero;     //! NPCの現在位置。
			Vector3 m_targetPosition = Vector3::Zero; //! ターゲットの現在位置。
			Vector3 m_difference = Vector3::Zero;     //! NPCとターゲットの差分。
			Vector3 m_sideMove = Vector3::Zero;       //! 旧横移動用。互換のため保持。

			WeaponType m_myWeapon = WeaponType::None; //! NPCの武器種。

			float m_distance = 0.0f;                  //! NPCとターゲットの距離。
			float m_attackRange = 0.0f;               //! NPCの攻撃開始距離。
		};
	}
}
