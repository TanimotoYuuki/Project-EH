#pragma once
/**
 * @file   PlayerHeavyAttackState.h
 * @brief  Playerの重攻撃を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/04/15
 * @detail 通常攻撃から派生。
 */

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerHeavyAttackState : public PlayerAttackBaseState
		{
		public:
			PlayerHeavyAttackState() = default;
			virtual ~PlayerHeavyAttackState() = default;

		protected:
			/* TemplateMethod。*/
			void PlayAttackAnimation() override;
			void OnEnterAttack() override;
			bool OnUpdateAttack() override;


		private:
			/* 弾を発射する。*/
			void FireHeavyBullet();


		private:
			Quaternion m_baseRot = Quaternion::Identity; //! 攻撃開始時の武器の角度を管理。
		};
	}
}
