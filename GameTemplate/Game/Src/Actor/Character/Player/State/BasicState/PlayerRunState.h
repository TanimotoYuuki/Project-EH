#pragma once
/**
* @file   PlayerRunState.h
* @brief  PlayerのRun状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/15
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsActor {
		class Player;
	}


	namespace nsState
	{
		class PlayerRunState : public IState<nsActor::Actor>
		{ 
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerRunState() = default;
			virtual ~PlayerRunState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override;


		/* セッター。*/
		public:
			/**
			 * @brief 走る速度を設定。
			 * @param speed 走る速度。
			 */
			inline void SetRunSpeed(float speed) 
			{
				m_runSpeed = speed;
			}

			/**
			 * @brief 現在の座標を設定。
			 * @param position 現在の座標。
			 */
			inline void SetPosition(const Vector3& position) 
			{
				m_currentPosition = position;
			}

			/**
			 * @brief 移動方向を設定。
			 * @param direction 移動方向のベクトル。
			 */
			inline void SetMoveDirection(const Vector3& direction) 
			{
				m_moveDirection = direction;
			}
			

		private:	
			nsActor::Player* m_player = nullptr;		//! Playerへのポインタ。


		private:
			float m_runSpeed;                           //! 走る速度。

			Vector3 m_moveVector = Vector3::Zero;       //! 走る際の移動ベクトル。
			Vector3 m_currentPosition = Vector3::Zero;  //! 現在の座標。
			Vector3 m_moveDirection = Vector3::Zero;    //! 移動ベクトル。
		};
	}
}
