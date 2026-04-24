#include "stdafx.h"
#include "PlayerMagicAttackState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerMagicAttackState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃の種類をセット。*/
			m_currentAttackType = AttackType::MagicAttack;

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::MagicAttack);

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerMagicAttackState::Update()
		{
			/* タイマーの更新。*/
			m_attackTimer++;

			/* nフレーム目でエフェクトを再生させる。*/
			if (m_attackTimer == 15)
				SpawnLaserEffect();

			/* アニメーションが終わったら待機状態へ戻る */
			if (m_attackTimer > 400 && !m_player->IsPlayAnimation())
				m_stateMachine->ChangeState(new PlayerIdleState());
		}


		void PlayerMagicAttackState::Exit()
		{
			/* ステート終了時にはエフェクトを停止指させる。*/
			if (m_laserEffect != nullptr)
			{
				m_laserEffect->Stop();
				m_laserEffect = nullptr;
			}

			PlayerAttackBaseState::Exit();
		}


		void PlayerMagicAttackState::SpawnLaserEffect()
		{
			m_spawnPosition = m_player->GetPosition();
			m_spawnPosition.y += 10.0f;
			m_spawnPosition += m_player->GetForwardVector() * 20.0f;

			/* エフェクトを生成する。*/
			m_laserEffect = m_player->GetEffectList().PlayEffect(nsEffect::MagicAttack, m_spawnPosition, Quaternion::Identity, Vector3::One * 10.0f);
		}
	}
}