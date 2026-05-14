#pragma once
/**
 * @file NPCTwinGunAttackState.h
 * @brief TwinGunNPCの攻撃ステートクラス。
 * @author Yamaguchi Hayato
 */

#include "NPCAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		/**
		 * @enum  NPCTwinGunPattern
		 * @brief NPCの双銃攻撃パターンの列挙型。
		 */
		enum class NPCTwinGunPattern : uint8_t
		{
			enRush,   //! 乱射攻撃。
			enHeavy,  //! 重攻撃（爆発弾）。
			enAir,    //! 空中攻撃。
			enNone,
		};

		class NPCTwinGunAttackState : public NPCAttackBaseState
		{
		public:
			NPCTwinGunAttackState() = default;
			virtual ~NPCTwinGunAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			bool RequestID(uint8_t& id) override { return false; }


		private:
			/* コンボ実行処理。*/
			void ExecuteCurrentCombo();

			/* 乱射コンボ。*/
			void ExecuteShootRush();

			/* 重攻撃コンボ。*/
			void ExecuteShootHeavy();

			/* 空中攻撃コンボ。*/
			void ExecuteShootAir();

			/* 移動処理。*/
			void UpdateMovement();

			/* 実行フロー。*/
			void ExecutionFlow();


		private:
			NPCTwinGunPattern m_currentPattern = NPCTwinGunPattern::enNone; //! 現在の攻撃パターンを保存する変数。
		};
	}
}