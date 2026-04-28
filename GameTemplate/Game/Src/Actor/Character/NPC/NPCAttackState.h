#pragma once
/**
* @file   NPCAttackState.h
* @brief  NPCの攻撃状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/04/28
*/

#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	namespace nsState
	{
		enum class NPCComboPattern : uint8_t
		{
			enMelee_Attack, //! B連打コンボ。
			enMelee_Heavy,  //! B → Xの攻撃コンボ。
			enMagic_Attack, //! RBの遠距離攻撃コンボ。
			enMagic_Heel,   //! RTの回復魔法コンボ。
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
			NPCBrain* m_brain = nullptr; //! NPCの親クラスのポインタ。


		private:
			int m_attackTimer = 0;       //! 攻撃のタイマー。

			NPCComboPattern m_currentPattern = NPCComboPattern::enNone;
			WeaponType m_myWeaponType = WeaponType::None;
		};

	}
}
