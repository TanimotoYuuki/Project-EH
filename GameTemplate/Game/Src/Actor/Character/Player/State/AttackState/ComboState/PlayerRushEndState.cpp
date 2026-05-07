#include "stdafx.h"
#include "PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"

#include "Src/Actor/Gun/Bullet/IGunBullet.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"


namespace nsApp
{
	namespace nsState
	{
		void PlayerRushEndState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃のタイプを設定する。*/
			m_currentAttackType = AttackType::RushAttack_End;

			/* アニメーションの再生。*/
			m_player->PlayWeaponAnimation(AttackType::RushAttack_End);

			m_attackTimer = 0;
		}

		void PlayerRushEndState::Update()
		{
			/* タイマーを加算。*/
			m_attackTimer++;

			if (m_attackTimer == 15)
			{
				m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();
				m_forwardDirection = m_player->GetForwardVector();

				/* 乱射弾（enRush）を指定。*/
				ConstructAndTransmitBulletRequest(BulletType::enRush);
			}

			/* アニメーションの再生が終わったらタメ攻撃状態に遷移。*/ 
			if (!m_player->IsPlayAnimation())
			{
				if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				{
					m_player->ResetSubWeapon();
					m_player->SetWeaponRotationAngle(Vector3::Front, -90.0f);
				}
				m_stateMachine->ChangeState(new PlayerChargeAttackState());
			}
		}
	}
}