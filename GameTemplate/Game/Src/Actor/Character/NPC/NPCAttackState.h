#pragma once
/**
 * @file   NPCAttackState.h
 * @brief  NPCの攻撃状態を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/04/28
 */

#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	namespace nsState
	{
		/*
		 * @enum NPCComboPattern。
		 * @brief NPCの攻撃コンボのパターンを管理する列挙型。
		 */
		enum class NPCComboPattern : uint8_t
		{
			enMelee_Rush,   //! 通常 -> 連続攻撃
			enMelee_Heavy,  //! 通常 -> 重攻撃
			enMelee_Push,   //! ダッシュ攻撃(突き進み)
			enMelee_Air,    //! ダッシュ -> 斬り上げ -> 空中攻撃（激アツコンボ！）
			enMagic_Attack, //! 遠距離魔法
			enMagic_Heal,   //! 回復魔法
			enNone          //! コンボなし。
		};

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
			/* 
			 * @brief コンボの種類ごとに実行関数を作成。
			 * @param body: NPCの体。
			 */
			/* B連打コンボ。 */
			void ExecuteMeleeRush(nsActor::Player* body);

			/* 重攻撃コンボ。*/
			void ExecuteMeleeHeavy(nsActor::Player* body);

			/* 中継コンボ。*/
			void ExecuteMeleePush(nsActor::Player* body);

			/* 空中コンボ。*/
			void ExecuteMeleeAir(nsActor::Player* body);

			/* RBの遠距離攻撃コンボ。*/
			void ExecuteMagicAttack(nsActor::Player* body);

			/* 回復魔法。*/
			void ExecuteMagicHeal(nsActor::Player* body);


		private:
			NPCBrain* m_brain = nullptr;                                //! NPCの親クラスのポインタ。


		private:
			int m_attackTimer = 0;                                      //! 攻撃のタイマー。

			NPCComboPattern m_currentPattern = NPCComboPattern::enNone; //! 現在の攻撃コンボのパターン。
			WeaponType m_myWeaponType = WeaponType::None;               //! NPCの現在の武器の種類。
		};

	}
}
