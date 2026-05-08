#pragma once
/**
* @file   PlayerSlashUpAttackState.h
* @brief  プレイヤーの斬り上げ攻撃状態。
* @author Yamaguchi Hayato
* @date   2026/03/18
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerSlashUpState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerSlashUpState() = default;
			virtual ~PlayerSlashUpState() = default; 


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id) override;


		private:
			/* 空中攻撃に遷移。*/
			void TransitionAirAttack();


			/* ジャンプ状態に遷移。*/
			void TransitionJumpState();


		public:
			/* 斬り上げ時の初速を設定。*/
			inline void SetJumpVelocity(float velocity) 
			{
				m_jumpVelocity = velocity;
			}

			/* */
			inline void SetSlashUpSpeed(Vector3 speed)
			{
				m_slashUpSpeed = speed;
			}


		private:
			float m_jumpVelocity;      //! 斬り上げ時の上昇と落下を管理。

			bool m_isSummoned = false; //! エフェクトを召喚したかどうかのフラグ。

			Vector3 m_slashUpSpeed;    //! 斬り上げ攻撃中の移動速度。
			Vector3 m_effectPosition;  //! エフェクトの位置。
		};
	}
}

