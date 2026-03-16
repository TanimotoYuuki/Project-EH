#pragma once

/**
* @file   PlayerNormalAttackState.h
* @brief  プレイヤーの通常攻撃状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/11
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerNormalAttackState : public PlayerAttackBaseState 
		{
		public:
			/* コンストラクタとデストラクタ。*/ 
			PlayerNormalAttackState() = default;
			virtual ~PlayerNormalAttackState() = default;


		public:
			/* ライフサイクル。*/
			/*ステートに入った際、1回処理を行う。*/
			void Enter() override;
			/* ステートにいる間、毎フレーム処理を行う。*/
			void Update() override;
		};
	}
}
