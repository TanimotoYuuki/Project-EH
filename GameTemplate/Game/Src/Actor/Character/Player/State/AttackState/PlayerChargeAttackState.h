#pragma once
/**
* @file   PlayerChargeAttackState.h
* @brief  プレイヤーのチャージ攻撃状態クラス
* @author YamaguchiHayato。
* @date   2026/03/16
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerChargeAttackState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerChargeAttackState() = default;
			virtual ~PlayerChargeAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;


		private:
			/* チャージ弾を発射させる。 */
			void FireChargeBullet();
		};
	}
}
