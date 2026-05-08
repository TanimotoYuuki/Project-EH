#pragma once
/**
 * @file   PlayerWalkState.h
 * @brief  プレイヤーの歩行状態を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/03/15
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"//


namespace nsApp
{
	namespace nsState
	{
		class PlayerWalkState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerWalkState() = default;
			virtual ~PlayerWalkState() = default;


		public:
			/* ライフサイクル。*/
			/* ステートに入る際、コールする処理。*/
			void Enter() override;
			/* ステートの最中、毎フレームコールする処理。*/
			void Update() override;
			/* ステートから出る際、コールする処理。*/
			void Exit() override {};
			/* ステートのリクエストIDを返す処理。*/
			bool RequestID(uint8_t& id) override;


		/* セッター。*/
		public:
			/* 歩行速度をセット。*/
			inline void SetWalkSpeed(float speed)
			{
				m_walkSpeed = speed;
			}


		private:
			nsActor::Player* m_player = nullptr;       /* プレイヤーのポインタ。*/


		private:
			float m_walkSpeed = 0.0f;                  /* 歩行速度。*/

			Vector3 m_moveVector = Vector3::Zero;      /* 歩行する際の移動ベクトル。*/
			Vector3 m_currentPosition = Vector3::Zero; /* 現在の位置。*/
			Vector3 m_moveDirection = Vector3::Zero;   /* 移動方向。*/
		};
	}
}
