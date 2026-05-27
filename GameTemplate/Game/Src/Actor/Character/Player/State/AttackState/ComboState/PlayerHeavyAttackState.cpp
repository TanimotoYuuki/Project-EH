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
            nsActor::Player* pPlayer = m_player;
            PlayerAttackBaseState::Update();

            if (pPlayer->GetCurrentWeapon() == WeaponType::TwinGun)
            {
                if (m_attackTimer == 8) 
                    FireHeavyBullet();
                

                if (!pPlayer->IsPlayAnimation()) {
                    m_stateMachine->ChangeState(new PlayerIdleState());
                    return;
                }
            }
            else 
            {
                if (m_attackTimer > 10 && !pPlayer->IsPlayAnimation())
                {
                    m_stateMachine->ChangeState(new PlayerIdleState()); 
                } 
            }
        }


        void PlayerHeavyAttackState::FireHeavyBullet()
        {
            nsActor::Player* pPlayer=m_player;
            m_spawnPosition = pPlayer->GetBonePosition(L"mixamorig:RightHand");
            m_forwardDirection = pPlayer->GetForwardVector();

            /* 爆発弾（enExplosive）を指定。*/
            ConstructAndTransmitBulletRequest(BulletType::enExplosive);
        }
    }
}