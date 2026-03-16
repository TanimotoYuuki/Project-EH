#pragma once
/**
* @file   PlayerAttackBaseState.h
* @brief  プレイヤーの攻撃状態の基底クラス。
* @author Yamaguchi Hayato
* @date   2026/03/11
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerAttackBaseState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerAttackBaseState() = default;
			virtual ~PlayerAttackBaseState() = default;


		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/
			void Enter() override;
			/* 毎フレーム更新する処理。*/
			void Update() override;
			/* 描画処理。*/
			void Exit() override;


		/* セッター。*/
		public:
			/* 攻撃の時間をセット。*/
			inline void SetAttackTimer(int timer)
			{
				m_attackTimer = timer;
			}


		protected:
			nsActor::Player* m_player = nullptr;    /* プレイヤーのポインタ。*/


		protected:
			int m_attackTimer = 0;                  /* 攻撃の時間管理用タイマー。*/
		};
	}
}
