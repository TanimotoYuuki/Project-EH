#include "stdafx.h"
#include "PlayerMagicAttackState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Magic/Parameter/MagicParameterTable.h"

namespace
{
	const auto ATTACK_TIMER_500 = 500;	//! 攻撃終了タイマーの値。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerMagicAttackState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定する。*/
			SetCurrentAttackType(AttackType::MagicAttack);

			/* 再生するアニメーションの種類をセット。*/
			m_player->PlayWeaponAnimation(AttackType::MagicAttack);
		}


		void PlayerMagicAttackState::OnEnterAttack()
		{
			/* チャージしていない場合は発動不可。*/
			const int chargeLevel = m_player->GetEffectScale();
			if (chargeLevel <= 0)
			{
				m_canExecuteMagicAttack = false;
				m_hasSpawnedLaserEffect = true;
				return;
			}

			/* 攻撃アニメーションを再生。*/
			m_canExecuteMagicAttack = true;

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();

			/* エフェクト生成フラグを初期化。*/
			m_hasSpawnedLaserEffect = false;
		}


		bool PlayerMagicAttackState::OnUpdateAttack()
		{
			/* チャージしていない場合は攻撃を終了する。*/
			if (!m_canExecuteMagicAttack)
			{
				/* 攻撃を終了してアイドルステートに遷移。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			if (!m_hasSpawnedLaserEffect && m_attackTimer >= 15)
			{
				/* レーザーエフェクトを生成する。*/
				SpawnLaserEffect();

				/* 魔法使用後はチャージをリセット。*/
				m_player->SetChargeLevel(0);

				/* エフェクト生成フラグを更新。*/
				m_hasSpawnedLaserEffect = true;
			}

			/* 攻撃終了タイマーが経過したらアイドルステートに遷移。*/
			if (m_attackTimer > ATTACK_TIMER_500 && !m_player->IsPlayAnimation())
			{
				/* 攻撃を終了してアイドルステートに遷移。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			if (m_attackTimer > ATTACK_TIMER_500)
			{
				/* 攻撃を終了してアイドルステートに遷移。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			return true;
		}


		void PlayerMagicAttackState::OnExitAttack()
		{
			/* 終了時にエフェクトを安全に消去 */
			if (m_laserEffect != nullptr)
			{
				m_laserEffect->Stop();
				m_laserEffect = nullptr;
			}
		}


		void PlayerMagicAttackState::SpawnLaserEffect()
		{
			/* エフェクトの生成位置をプレイヤーの前方に設定。*/
			m_spawnPosition = m_player->GetPosition();
			m_spawnPosition.y += 10.0f;
			m_spawnPosition += m_player->GetForwardVector() * 20.0f;

			/* エフェクトを生成する。*/
			m_laserEffect = m_player->GetEffectList().PlayEffect(nsEffect::MagicAttack, m_spawnPosition, Quaternion::Identity, Vector3::One * 10.0f);
		}
	}
}