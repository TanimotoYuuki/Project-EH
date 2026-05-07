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


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			bool RequestID(uint8_t& id) override { return false; };


		private:
			/* 弾を発射する。*/
			void FireHeavyBullet();


		private:
			Vector3 m_spawnPosition = Vector3::Zero; //! 弾の生成位置を管理。

			Quaternion m_baseRot;					 //! 攻撃開始時の武器の角度を管理。
		};
	}
}
