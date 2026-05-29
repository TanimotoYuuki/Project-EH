#include "stdafx.h"
#include "PlayerHeavyAttackState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerHeavyAttackState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃の種類を設定。*/
			m_currentAttackType = AttackType::HeavyAttack;

			/* 武器アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::HeavyAttack);

			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				m_player->SetWeaponRotationAngle(Vector3::Front, -90.0f);

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


        void PlayerHeavyAttackState::Update()
        {
            PlayerAttackBaseState::Update();

            if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
            {
                if (m_attackTimer == 8) 
                    FireHeavyBullet();
                

                if (!m_player->IsPlayAnimation()) {
                    m_stateMachine->ChangeState(new PlayerIdleState());
                    return;
                }
            }
            else 
                if (m_attackTimer > 10 && !m_player->IsPlayAnimation()) 
                    m_stateMachine->ChangeState(new PlayerIdleState);
        }


        void PlayerHeavyAttackState::FireHeavyBullet()
        {
            m_spawnPosition = m_player->GetBonePosition(L"mixamorig:RightHand");
            m_forwardDirection = m_player->GetForwardVector();

            /* 爆発弾（enExplosive）を指定。*/
            ConstructAndTransmitBulletRequest(BulletType::enExplosive);
        }
    }
}