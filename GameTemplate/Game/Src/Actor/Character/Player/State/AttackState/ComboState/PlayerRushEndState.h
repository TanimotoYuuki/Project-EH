#pragma once
/**
* @file   PlayerRushEndState.h
* @brief  プレイヤーの連続攻撃のループ状態を管理するクラス。
*         アニメーション終了後はチャージ攻撃状態に遷移して1コンボとする。
* @author Yamaguchi Hayato
* @date   2026/03/17
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerRushEndState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerRushEndState() = default;
			virtual ~PlayerRushEndState() = default;


		protected:
			void PlayAttackAnimation() override;
			void OnAttackTick() override;
			bool OnUpdateAttack() override;
		};
	}
}
