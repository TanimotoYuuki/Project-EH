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
			/* 移動速度をセット。*/
			inline void SetRunSpeed(float speed) 
			{
				m_runSpeed = speed;
			}

			/* 移動速度を設定。*/
			inline void SetPosition(const Vector3& position) 
			{
				m_currentPosition = position;
			}

			/* 移動ベクトルを設定。*/
			inline void SetMoveDirection(const Vector3& direction) 
			{
				m_moveDirection = direction;
			}
			

		/* ゲッター。*/
		public:
		


		private:	
			nsActor::Player* m_player = nullptr;


		private:
			float m_runSpeed;                           /* 走る速度。*/

			Vector3 m_moveVector = Vector3::Zero;       /* 走る際の移動ベクトル。*/
			Vector3 m_currentPosition = Vector3::Zero;  /* 現在の座標。*/
			Vector3 m_moveDirection = Vector3::Zero;    /* 移動ベクトル。*/
		};
	}
}
