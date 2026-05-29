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
			void Exit() override;

		/* ヘルパー。*/
		private:
			/* 回復魔法エフェクトの再生。*/
			void PlayHeelMagicEffect();

			/* 回復魔法エフェクトの再生。(パーティクル部分)*/
			void PlayHeelMagicParticleEffect();

			/* 回復エフェクトを徐々に大きくする。*/
		    void ComputeHeelEffectScale()
			{
				if (m_heelEffect != nullptr)
				{
					m_heelEffectScalling = static_cast<float>(m_attackTimer * 0.05f);
					m_heelEffect->SetScale(Vector3::One * m_chargeLevel * m_heelEffectScalling * 10.0f);
				}
			}

			/**
			 * @brief 回復魔法の効果を実行する関数。
			 */
			void ExecuteAreaHeal();


		private:
			nsK2EngineLow::EffectEmitter* m_heelEffect = nullptr;
			nsK2EngineLow::EffectEmitter* m_magicEffect = nullptr;
			nsK2EngineLow::EffectEmitter* m_particleEffect = nullptr;


		private:
			Vector3 m_heelEffectPosition;	    //! 回復魔法エフェクトの位置。
			Vector3 m_particleEffectPosition;   //! パーティクルエフェクトの位置。

			float m_heelEffectScalling = 1.0f;  //! 回復魔法エフェクトを拡大率。
			float m_chargeLevel = 0.0f;         //! チャージ段階。
			float m_distance = 0.0f;            //! プレイヤーと回復対象の距離。

			int m_healAmount = 0;			    //! 回復量を管理する変数。
		};
	}
}
