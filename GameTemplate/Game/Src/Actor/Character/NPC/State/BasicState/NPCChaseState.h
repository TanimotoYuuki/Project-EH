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
			 * @param helpTarget: 助ける対象。
			 */
			bool ExecuteHelpAction(nsActor::Player* helpTarget);


			/**
			 * @brief 距離をつめる対象を設定する。
			 * @param target: 距離をつめる対象。
			 */
			void ExecuteChaseAction(nsActor::ICharacter* target);


			/* 攻撃ステートへの遷移を関数化。*/
			void TransitionToAttackState();


			/* 
			 * @brief 距離計算メソッド。
			 * @param targetObject: 距離を計算する対象。
			 */
			inline void ComputeDistance(nsActor::ICharacter* targetObject)
			{
				/* 距離の計算。*/
				m_difference = targetObject->GetPosition() - m_body->GetPosition();
				/* y軸は加味しない。*/
				m_difference.y = 0.0f;
				/* 距離ベクトルの長さを保存する。*/
				m_distance = m_difference.Length();
			}

			/**
			 * @brief 武器の種類に応じて攻撃開始距離を変える処理。
			 * @param type 武器の種類。
			 */
			float CharacterToBeChosen(WeaponType type) const;


		private:
			NPCBrain* m_brain = nullptr;			  //! NPCの親クラスのポインタ。
			nsActor::Player* m_body = nullptr;	      //! NPCのボディクラスのポインタ。
			VirtualInputAdapter* m_vInput = nullptr;  //! NPCの入力クラスのポインタ。


		private:
			Vector3 m_myPosition = Vector3::Zero;     //! NPCの現在位置。
			Vector3 m_targetPosition = Vector3::Zero; //! ターゲットの現在位置。
			Vector3 m_difference = Vector3::Zero;     //! NPCとターゲットの位置の差分。
			Vector3 m_sideMove = Vector3::Zero;       //! NPCの横移動方向。

			WeaponType m_myWeapon = WeaponType::None; //! NPCの武器の種類。

			float m_distance = 0.0f;				  //! NPCとターゲットの距離。
			float m_attackRange = 0.0f;			      //! NPCの攻撃開始距離。
		};
	}
}
