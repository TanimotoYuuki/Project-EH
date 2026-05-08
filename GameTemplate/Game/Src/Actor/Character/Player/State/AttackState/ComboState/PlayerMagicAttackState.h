#pragma once
/**
* @file   PlayerMagicAttackState.h
* @brief  プレイヤーの魔法攻撃ステート。
* @author Yamaguchi Hayato。
* @date   2026/04/21
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsActor	{
		class MagicProjectotile;
	}

	namespace nsState
	{
		class PlayerMagicAttackState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerMagicAttackState() = default;
			virtual ~PlayerMagicAttackState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;


		private:
			/* レーザーエフェクトを生成する。*/
			void SpawnLaserEffect();


		private:
			nsK2EngineLow::EffectEmitter* m_laserEffect = nullptr; //! 発射する魔法の弾。
		};
	}
}
