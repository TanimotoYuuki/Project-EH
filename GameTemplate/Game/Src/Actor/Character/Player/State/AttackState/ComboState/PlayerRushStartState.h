#pragma once
/*
* @file   PlayerRushStartState.h
* @brief  プレイヤーの連続攻撃開始状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/17
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerRushStartState  :public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerRushStartState() = default;
			virtual ~PlayerRushStartState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			bool RequestID(uint8_t& id) override;


		private:
			/* 前進する処理。*/
			void MoveForward();

			/* ミサイルを召喚。*/
			void SummonMissile();


		private:
			int m_loopCount = 0;                  //! 連続攻撃のループ回数を管理するカウンター。

			float m_forwardSpeed = 0.0f;          //! 連続攻撃をする際、前方への移動速度。

			Vector3 m_moveVector = Vector3::Zero; //! 前進する際の移動ベクトル。
			Vector3 m_spawnPos = Vector3::Zero;   //! ミサイルを召喚する位置。
		};
	}
}
