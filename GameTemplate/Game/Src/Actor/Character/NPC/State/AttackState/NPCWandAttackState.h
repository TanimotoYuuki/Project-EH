#pragma once

/**
 * @file NPCWandAttackState.h
 * @brief WandNPCの攻撃ステートクラス。
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

		class NPCWandAttackState : public IState<NPCBrain>
		{
		public:
			NPCWandAttackState() = default;
			virtual ~NPCWandAttackState() = default;

		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;
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

			Vector3 m_diff = Vector3::Zero;                                 //! ターゲットとの位置差ベクトルを保存する変数。
			Vector3 m_pushBackDir = Vector3::Zero;                          //! めり込み防止のための押し戻す方向ベクトルを保存する変数。
			Vector3 m_currentPosition = Vector3::Zero;                      //! 現在のNPCの位置を保存する変数。

			NPCWandPattern m_currentPattern = NPCWandPattern::enNone;       //! 現在の攻撃パターンを保存する変数。
		};
	}
}