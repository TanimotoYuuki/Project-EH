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


		protected:
			/* 親クラスから呼ばれる処理。*/
			void PlayAttackAnimation() override;
			void OnAttackTick() override;
			bool OnUpdateAttack() override;
			bool OnRequestAttackID(uint8_t& id) override;


		private:
			/*
			 * @brief WandCharacterを使用している場合、攻撃の際にミサイルを召喚する処理。
			 */
			void SummonMissile();

			/*
			 * @brief 銃弾を発射させる処理。
			 */
			void FireGunBullet();
		};
	}
}
