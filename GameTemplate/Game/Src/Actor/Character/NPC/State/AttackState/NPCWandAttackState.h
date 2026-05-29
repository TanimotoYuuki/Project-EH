#pragma once

/**
 * @file NPCWandAttackState.h
 * @brief WandNPCの攻撃ステートクラス。
 * @author Yamaguchi Hayato。
 * @date 2026/04/29
 */

#include "NPCAttackBaseState.h"
#include <cstdint>

namespace nsApp
{
	namespace nsState
	{
		/**
		 * @enum  NPCWandPattern
		 * @brief NPCの杖攻撃パターンの列挙型。
		 */
		enum class NPCWandPattern : uint8_t
		{
			enMagicAttack, //! 魔法攻撃（レーザー）。
			enMagicHeal,   //! 回復魔法。
			enAir,         //! 空中攻撃（魔法弾ばら撒き）。
			enNone,
		};

		class NPCWandAttackState : public NPCAttackBaseState
		{
		public:
			NPCWandAttackState() = default;
			virtual ~NPCWandAttackState() = default;

		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			bool RequestID(uint8_t& id) { return false; };


		private:
			/* コンボ実行処理。*/
			void ExecuteCurrentCombo();

			/* 魔法攻撃コンボ。*/
			void ExecuteMagicAttack();

			/* 回復魔法コンボ。*/
			void ExecuteMagicHeal();

			/* 空中攻撃コンボ。*/
			void ExecuteMeleeAir();

			/* 移動処理。*/
			void UpdateMovement();

			/* 実行フロー。*/
			void ExecutionFlow();


		private:
			NPCWandPattern m_currentPattern = NPCWandPattern::enNone;       //! 現在の攻撃パターンを保存する変数。
		};
	}
}