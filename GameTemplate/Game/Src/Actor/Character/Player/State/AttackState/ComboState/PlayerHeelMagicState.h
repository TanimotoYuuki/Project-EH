#pragma once
/**
* @file   PlayerHeelMagicState.h
* @brief  プレイヤーの回復魔法状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/04/21
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerHeelMagicState : public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerHeelMagicState() = default;
			virtual ~PlayerHeelMagicState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;


		/* ヘルパー。*/
		private:
			/* 回復魔法エフェクトの再生。*/
			void PlayHeelMagicEffect();

			/* 回復魔法エフェクトの再生。(パーティクル部分)*/
			void PlayHeelMagicParticleEffect();

			/* 回復エフェクトを徐々に大きくする。*/
		     void ComputeHeelEffectScale()
			{
				if (m_heelEffect != nullptr or m_particleEffect != nullptr)
				{
					m_heelEffectScalling = static_cast<float>(m_attackTimer * 0.05f);
					m_magicEffect = m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic_Particle, m_particleEffectPosition, Quaternion::Identity, Vector3::One * m_chargeLevel * 2.0f);
				}
			}


		private:
			nsK2EngineLow::EffectEmitter* m_heelEffect = nullptr;
			nsK2EngineLow::EffectEmitter* m_magicEffect = nullptr;
			nsK2EngineLow::EffectEmitter* m_particleEffect = nullptr;


		private:
			Vector3 m_heelEffectPosition;	    //! 回復魔法エフェクトの位置。
			Vector3 m_particleEffectPosition;   //! パーティクルエフェクトの位置。

			float m_heelEffectScalling = 1.0f;  //! 回復魔法エフェクトを拡大率。
			float m_chargeLevel = 0.0f;         //! チャージ段階。
		};
	}
}
