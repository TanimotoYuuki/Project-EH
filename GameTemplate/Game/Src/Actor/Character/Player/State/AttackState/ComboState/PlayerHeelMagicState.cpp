#include "stdafx.h"
#include "PlayerHeelMagicState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Effect/EffectList.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerHeelMagicState::Enter()
		{
			/* キャストしてプレイヤーを取得 */
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃の種類をセットする。*/
			m_currentAttackType = AttackType::HeelMagic;

			/* 回復もチャージ解放のアニメーションを流用 */
			m_player->PlayWeaponAnimation(AttackType::HeelMagic);

			/* タイマーの初期化。*/
			SetAttackTimer(0);
		}

		void PlayerHeelMagicState::Update()
		{
			/* タイマーの更新。*/
			m_attackTimer++;

			/* 徐々にエフェクトを大きくする。*/
			ComputeHeelEffectScale();

			/* タイマーごとに再生するエフェクトを分岐させる。*/
			if (m_attackTimer == 15)
				PlayHeelMagicEffect();
			else if (m_attackTimer == 25)
				PlayHeelMagicParticleEffect();


			/* アニメーションが終わったら待機状態へ戻る */
			if (m_attackTimer > 25 && !m_player->IsPlayAnimation())
				m_stateMachine->ChangeState(new PlayerIdleState());
		}


		void PlayerHeelMagicState::PlayHeelMagicEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_heelEffectPosition = m_player->GetPosition();
			m_heelEffectPosition.y += 2.0f;
			/* エフェクトを再生。*/
			m_heelEffect =  m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic, m_heelEffectPosition, Quaternion::Identity, Vector3::One  *20.0f);
		}


		void PlayerHeelMagicState::PlayHeelMagicParticleEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_particleEffectPosition = m_player->GetPosition();
			m_particleEffectPosition.y += 2.0f;
			/* エフェクトを再生。*/
			m_heelEffect = m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic_Particle, m_heelEffectPosition, Quaternion::Identity, Vector3::One * 20.0f);
		}
	}
}