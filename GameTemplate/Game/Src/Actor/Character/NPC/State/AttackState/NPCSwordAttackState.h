#pragma once

/**
 * @file NPCSwordAttackState.h
 * @brief SwordNPCの攻撃ステートクラス。
 * @author Yamaguchi Hayato。
 * @date 2026/04/29
 */

#include "NPCAttackBaseState.h"

namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	namespace nsState
	{
		/**
		 * @enum  NPCSwordPattern
		 * @brief NPCの剣攻撃パターンの列挙型。
		 */
		enum class NPCSwordPattern : uint8_t
		{
			enRush, //! 突進攻撃。
			enPush, //! 中継コンボ。
			enAir,  //! 空中攻撃。
			enNone,
		};

		class NPCSwordAttackState : public NPCAttackBaseState
		{
		public:
			NPCSwordAttackState() = default;
			virtual ~NPCSwordAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;


		private:
			/* コンボ実行処理。*/
			void ExecuteCurrentCombo();

			/* 連続攻撃コンボ。*/
			void ExecuteMeleeRush();

			/* 中継コンボ。*/
			void ExecuteMeleePush();

			/* 空中攻撃コンボ。*/
			void ExecuteMeleeAir();

			/* 実行フロー。*/
			void ExecutionFlow();

			/* 移動処理。*/
			void UpdateMovement();


		private:
			NPCSwordPattern m_currentPattern = NPCSwordPattern::enNone;     //! 現在の攻撃パターンを保存する変数。
		};
	}
}