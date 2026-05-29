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


		protected:
			// --- Template Method  ---
			void PlayAttackAnimation() override;
			void OnEnterAttack() override;
			bool OnUpdateAttack() override;
			void OnAttackTick() override;
			void OnExitAttack() override;


		private:
			/* レーザーエフェクトを生成する。*/
			void SpawnLaserEffect();


		private:
			nsK2EngineLow::EffectEmitter* m_laserEffect = nullptr; //! 発射する魔法の弾。


		private:
			bool m_hasSpawnedLaserEffect = false;	//! レーザーエフェクトを生成済みか。
			bool m_canExecuteMagicAttack = false;   //! 魔法攻撃を実行できるか。
		};
	}
}
