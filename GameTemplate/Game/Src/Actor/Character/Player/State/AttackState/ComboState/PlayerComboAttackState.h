#pragma once
/**
* @file   PlayerComboAttackState.h
* @brief  プレイヤーのコンボ攻撃状態の1段目を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/17
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerComboAttackState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerComboAttackState() = default;
			virtual ~PlayerComboAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			bool RequestID(uint8_t& id) override;

		};
	}
}
