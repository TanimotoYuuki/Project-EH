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


		public:
		    /* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;

			/* テーブルから判定を呼び出す。*/
			bool RequestID(uint8_t& id) override;


		/* セッター。*/
		public:
			/* 前進する際の速度を設定。*/
			inline void SetForwardSpeed(float speed)
			{
				m_forwardSpeed = speed;
			}


		private:
			/* 突き進む処理。*/
			void MoveForward();


		private:
			int m_loopTimer = 0;                  //! 連続攻撃のループ回数を管理。
			
			float m_forwardSpeed = 0.0f;          //! 突き進む速度。

			Vector3 m_moveVector = Vector3::Zero; //! 突き進む方向を管理。

		};
	}
}

