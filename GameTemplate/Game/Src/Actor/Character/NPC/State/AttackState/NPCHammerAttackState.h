#pragma once

/**
 * @file NPCHammerAttackState.h
 * @brief HammerNPCの攻撃ステートクラス。
 * @author Yamaguchi Hayato。
 * @date 2026/04/29
 */

#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Player/Player.h"
#include <cstdint>

namespace nsApp
{
	namespace nsActor {
		class Player;
		class Sandbag;
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

		class NPCHammerAttackState : public IState<NPCBrain>
		{
		public:
			NPCHammerAttackState() = default;
			virtual ~NPCHammerAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;
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

			/* ヘルパー群。*/
			void ResetVirtualInputs();
			void PreventClipping(nsActor::Sandbag* target);
			void UpdateFacingDirection();
			void UpdateMovement();
			void ExecutionFlow();

			/* 
			 * @brief ターゲットとの距離を計算する。
			 * @param targetObject ターゲットのオブジェクト。
			 */
			inline void ComputeDistance(nsActor::Sandbag* targetObject)
			{
				m_diff = targetObject->GetPosition() - m_body->GetPosition();
				m_distance = m_diff.Length();
			}


		private:
			NPCBrain* m_brain = nullptr;                                    //! このステートを所有するNPCのブレインへのポインタ。
			nsActor::Player* m_body = nullptr;                              //! 自身のプレイヤー実体へのポインタ。
			PlayerInput* m_input = nullptr;                                 //! プレイヤーの入力クラスへのポインタ。


		private:
			int m_attackTimer = 0;                                          //! 攻撃の進行を管理するタイマー。
			int m_randomPattern = 0;                                        //! 攻撃パターンの抽選結果を保存する変数。

			float m_distance = 0.0f;                                        //! ターゲットとの距離を保存する変数。
			float m_stickX = 0.0f;                                          //! スティックのX軸の値を保存する変数。
			float m_stickZ = 0.0f;                                          //! スティックのZ軸の値を保存する変数。

			bool m_isAttacking = false;                                     //! 攻撃中かどうかを判定するフラグ。
			bool m_isDashAttack = false;                                    //! ダッシュ攻撃かどうかを判定するフラグ。

			Vector3 m_diff = Vector3::Zero;                                 //! ターゲットとの位置差ベクトルを保存する変数。
			Vector3 m_pushBackDir = Vector3::Zero;                          //! めり込み防止のための押し戻す方向ベクトルを保存する変数。
			Vector3 m_currentPosition = Vector3::Zero;                      //! 現在のNPCの位置を保存する変数。

			NPCHammerPattern m_currentPattern = NPCHammerPattern::enNone;   //! 現在の攻撃パターンを保存する変数。
		};
	}
}