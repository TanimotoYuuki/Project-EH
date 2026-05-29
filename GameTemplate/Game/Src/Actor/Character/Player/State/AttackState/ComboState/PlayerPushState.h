#pragma once
/**
* @file   PlayerPushState.h
* @brief  突き進む攻撃をする状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/18
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerPushState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerPushState() = default;
			virtual ~PlayerPushState() = default;


		protected:
			/* TemplateMethod。*/
			void PlayAttackAnimation() override;
			void OnEnterAttack() override;
			bool OnUpdateAttack() override;
			void OnExitAttack() override;
			bool OnRequestAttackID(uint8_t& id) override;


		/* セッター。*/
		public:
			/* 
			 * @brief 前進する際の速度を設定。
			 * @param 前進する速度。
			 */
			inline void SetForwardSpeed(float speed)
			{
				m_forwardSpeed = speed;
			}


		private:
			/* 突き進む処理。*/
			void MoveForward();

			/* 弾を発射。*/
			void FireDashBullet();
			
			/* 速度の減衰率に応じて複数のステートに遷移。*/
			bool TransitionMultiState();


		private:
			int m_loopTimer = 0;                        //! 連続攻撃のループ回数を管理。
			
			float m_forwardSpeed = 0.0f;                //! 突き進む速度。

			Vector3 m_moveVector = Vector3::Zero;	    //! 突き進む方向を管理。
		};
	}
}

