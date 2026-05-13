#pragma once

/**
 * @file NPCHammerAttackState.h
 * @brief HammerNPCの攻撃ステートクラス。
 * @author Yamaguchi Hayato。
 * @date 2026/04/29
 */

#include "NPCAttackBaseState.h"
#include <cstdint>

namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	namespace nsState
	{
		/**
		 * @enum  NPCHammerPattern
		 * @brief NPCのハンマー攻撃パターンの列挙型。
		 */
		enum class NPCHammerPattern : uint8_t
		{
			enHeavy, //! 重攻撃。
			enPush,  //! 突き進み攻撃。
			enAir,   //! 空中攻撃。
			enNone,
		};

		class NPCHammerAttackState : public NPCAttackBaseState
		{
		public:
			NPCHammerAttackState() = default;
			virtual ~NPCHammerAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			bool RequestID(uint8_t& id) { return false; };


		private:
			/* コンボ実行処理。*/
			void ExecuteCurrentCombo();

			/* 重攻撃コンボ。*/
			void ExecuteMeleeHeavy();

			/* 中継（ダッシュ）コンボ。*/
			void ExecuteMeleePush();

			/* 空中攻撃コンボ。*/
			void ExecuteMeleeAir();

			/* 移動処理。*/
			void UpdateMovement();

			/* 実行フロー。*/
			void ExecutionFlow();


		private:
			NPCHammerPattern m_currentPattern = NPCHammerPattern::enNone;   //! 現在の攻撃パターンを保存する変数。
			bool m_isDashAttack = false;
		};
	}
}