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
#include "Src/Actor/Character/NPC/Movement/NPCCombatRangeHelper.h"

namespace nsApp
{
	namespace nsActor
	{
		class Player;
	}

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
			void Exit() override;
			bool RequestID(uint8_t &id) override { return false; };

		private:
			/**
			 * @brief 対象へ近づき、攻撃可能なら攻撃ステートへ遷移する。
			 * @param target 追跡対象。
			 */
			void ExecuteChaseAction(nsActor::ICharacter *target);

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
			void ComputeDistance(nsActor::ICharacter *targetObject);

			/**
			 * @brief 対象から離れるための移動入力を入れる。
			 */
			void MoveAwayFromTarget();

			/**
			 * @brief 回復エリアへ移動する。
			 * @return 回復エリアへ移動できる場合は true、できない場合は false。
			 */
			bool TryMoveToHealArea();

			/**
			 * @brief 移動方向に味方分離ベクトルを混ぜる。
			 * @param moveDirection 元の移動方向。
			 * @return 分離を混ぜた移動方向。
			 */
			Vector3 BlendSeparation(const Vector3 &moveDirection) const;

		private:
			NPCBrain *m_brain = nullptr;			 //! NPCの親クラス。
			nsActor::Player *m_body = nullptr;		 //! NPC本体。
			VirtualInputAdapter *m_vInput = nullptr; //! NPCの仮想入力。

		private:
			Vector3 m_difference = Vector3::Zero; //! NPCとターゲットの差分。

			WeaponType m_myWeapon = WeaponType::None; //! NPCの武器種。

			float m_distance = 0.0f;	//! NPCとターゲットの距離。
			float m_attackRange = 0.0f; //! NPCの攻撃開始距離。
		};
	}
}
