#include "stdafx.h"
#include "PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"

#include "Src/Actor/Gun/Bullet/IGunBullet.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"


namespace nsApp
{
	namespace nsState
	{
		void PlayerRushEndState::PlayAttackAnimation()
		{
			/* 攻撃の種類をセット。*/
			m_currentAttackType = AttackType::RushAttack_End;

			/* 再生するアニメーションをセット。*/
			m_player->PlayWeaponAnimation(AttackType::RushAttack_End);
		}


		void PlayerRushEndState::OnAttackTick()
		{
			/* 15フレーム目に処理。*/
			if (m_attackTimer == 15)
			{
				m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();
				m_forwardDirection = m_player->GetForwardVector();
				ConstructAndTransmitBulletRequest(BulletType::enRush);
			}
		}


		bool PlayerRushEndState::OnUpdateAttack()
		{
			/* アニメーションの再生終了を待つ。*/
			if (!m_player->IsPlayAnimation())
			{
				/* 銃の場合。*/
				if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				{
					/* サブウェポンをリセットする。*/
					m_player->ResetSubWeapon();
					/* 武器の角度をリセットする。*/
					m_player->SetWeaponRotationAngle(Vector3::Front, -90.0f);
				}

				/* 乱射攻撃のフィニッシュとしてチャージ攻撃を放つ。*/
				m_stateMachine->ChangeState(new PlayerChargeAttackState());
				return true;
			}

			return false;
		}
	}
}