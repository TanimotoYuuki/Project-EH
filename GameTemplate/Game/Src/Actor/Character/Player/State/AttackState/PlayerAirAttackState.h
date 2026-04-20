#pragma once
/**
* @file   PlayerAirAttackState.h
* @brief  プレイヤーの空中攻撃ステート。
* @author Yamaguchi Hayato。
* @date   2026/06/15
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"


namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	namespace nsState
	{
		class PlayerAirAttackState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerAirAttackState() = default;
			virtual ~PlayerAirAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override {};


		private:
			/**
			* @brief 空中での移動処理。
			*/
			void UpdateAirMovement();

			/**
			* @brief 着地判定。
			*/
			bool CheckLanding();

			/**
			 * @brief  エフェクトの生成。
			 * @detail 着磁時の衝撃波エフェクトを生成する。
			*/
			void CreateShockWaveEffect();

			/**
			* @brief  アニメーションの終了判定。
			* @return アニメーションが終了している場合はtrue、そうでない場合はfalse。
			*/
			bool CheckAnimationEndTransition();


		/* セッター。*/
		public:
			/* 攻撃タイマーを設定。*/
			inline void SetAttackTimer(int timer)
			{
				m_attackTimer = timer;
			}

			/* 落下速度を設定。*/
			inline void SetFallVelocity(float velocity)
			{
				m_fallVelocity = velocity;
			}

			/* 重力を設定。*/
			inline void SetGravity(float gravity)
			{
				m_gravity -= gravity;
			}

			/* 移動速度を設定。*/
			inline void SetMoveSpeed(const Vector3 moveSpeed)
			{
				m_moveSpeed = moveSpeed;
			}

			/* 空中で動ける速度を設定。*/
			inline void SetAirMoveSpeed(float speed)
			{
				m_airMoveSpeed = speed;
			}


		private:
			nsActor::Player* m_player = nullptr;           //! プレイヤーのポインタ。


		private:
			int m_attackTimer = 0;                         //! 攻撃のタイマー。

			float m_airMoveSpeed = 0.0f;
			float m_fallVelocity = 0.0f;                   //! 落下速度。
			float m_gravity = 0.0f;                        //! 重力の強さ。

			bool m_isLanding = false;                      //! 着地したかどうかのフラグ。

			Vector3 m_moveSpeed = Vector3::Zero;           //! 空中での移動速度。
			Vector3 m_currentAirMoveSpeed = Vector3::Zero; //! 空中での現在の移動速度。
			Vector3 m_landingPosition = Vector3::Zero;     //! 着地した位置。
		};
	}
}
