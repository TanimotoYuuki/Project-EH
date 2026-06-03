#pragma once

/**
 * @file   NPCAttackBaseState.h
 * @brief  NPCの攻撃の基底クラス。
 * @author Yamaguchi Hayato。
 * @date   2026/05/11
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/NPC/Component/NPCCombatHelper.h"

namespace nsApp
{
	namespace nsState
	{
		class NPCAttackBaseState : public IState<NPCBrain>
		{
		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override = 0;
			void Exit() override;


		protected:
			/* 
			 * @brief 攻撃タイマーを設定。
			 * @param timer 設定するタイマーの値。
			 */
			inline void SetAttackTimer(int timer)
			{
				m_attackTimer = timer;
			}


		protected:
		/* 共通処理。*/
			/**
			 * @brief ターゲットとの距離を計算する処理。
			 * @param targetObject ターゲットのオブジェクト。
			 */
			void ComputeDistance(nsActor::ICharacter* targetObject);

			/**
			 * @brief ターゲットとの位置の差分を元にした押し返す方向を計算する処理。
			 * @param target ターゲットのオブジェクト。
			 */
			void PreventClipping(nsActor::ICharacter* target);

			/**
			 * @brief ターゲットの方を向く処理。
			 */
			void UpdateFacingDirection();

			/**
			 * @brief 攻撃のタイマーをリセットする処理。
			 */
			void ResetVirtualInputs();

			/**
			 * @brief 攻撃のタイマーを更新する処理。
			 * @return 攻撃のタイマーが0以下になったらtrue。
			 */
			bool CheckHelpTransition();


		protected:
			/* 共通インスタンス。*/
			nsApp::NPCBrain* m_npcBrain = nullptr;		   //! NPCの思考部分を管理するクラス。
			nsActor::Player* m_getBody = nullptr;		   //! プレイヤーのインスタンス。
			VirtualInputAdapter* m_virtualInput = nullptr; //! 仮想コントローラー。


		protected:
			int m_attackTimer = 0;					       //! 攻撃のタイマー。
			int m_randomPattern = 0;    				   //! 攻撃パターンの乱数を保存する変数。

			float m_distance = 0.0f;					   //! ターゲットとの距離。
			float m_stickX = 0.0f;					       //! スティックのX軸の値。
			float m_stickZ = 0.0f;						   //! スティックのZ軸の値。

			bool m_isAttacking = false;					   //! ターゲットの方を向いているか。

			Vector3 m_diff = Vector3::Zero;				   //! ターゲットとの位置の差分。
			Vector3 m_pushBakeDirection = Vector3::Zero;   //! ターゲットとの位置の差分を元にした押し返す方向。
			Vector3 m_currentPosition = Vector3::Zero;     //! NPCの現在の位置。
		};
	}
}
