#pragma once
/**
* @file   PlayerHitState.h
* @brief  ダメージを受けた際の状態を管理するクラス。
* @author Yamaguchi Hayato。
* @date   2026/03/16
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
		class PlayerHitState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerHitState() = default;
			virtual ~PlayerHitState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override {};

			/* ステートを遷移させる。*/
			bool RequestID(uint8_t& id) override;


		/* セッター。*/
		public:
			/* ダメージを受けてからの時間を設定。*/
			inline void SetHitTimer(int timer)
			{
				m_hitTimer = timer;
			}

			/* 起き上がりフラグを設定。*/
			inline void SetGetUpFlag(bool flag)
			{
				m_isGetUP = flag;
			}

			/* 吹っ飛ぶ速度を設定。
			* @param speed 吹っ飛ぶ速度のベクトル。
			* 今回はx軸を制御。
			*/
			inline void SetKnockBackSpeed(const Vector3 speed)
			{
				m_knockBackSpeed.x = speed.x;
			}

			/* 吹っ飛ぶ初速を設定。*/
			inline void SetKnockBackVelocity(float velocity)
			{
				m_knockBackVelocity = velocity;
			}


		private:
			nsActor::Player* m_player;  /* プレイヤーの実態へのポインタ。*/


		private:	
			int m_hitTimer = 0;                       /* ダメージを受けてからの経過時間。*/

			bool m_isGetUP = false;                   /* ダメージを受けてから起き上がるまでのフラグ。*/

			float m_knockBackVelocity = 0.0f;         /* 吹っ飛びの初速。*/

			Vector3 m_knockBackSpeed = Vector3::Zero; /* 吹っ飛びの速度。*/

		};
	}
}