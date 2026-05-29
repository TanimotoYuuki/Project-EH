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


		protected:
			/* 親クラスから呼ばれる処理。*/
			void PlayAttackAnimation() override;
			void OnAttackTick() override;


		private:
			/* 
			 * @broef チャージ弾を発射させる。
			 */
			void FireChargeBullet();
		};
	}
}
