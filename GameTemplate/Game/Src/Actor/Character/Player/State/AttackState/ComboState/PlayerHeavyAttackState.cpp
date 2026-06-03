#include "stdafx.h"
#include "PlayerHeavyAttackState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace nsApp
{
	namespace nsState
	{
        void PlayerHeavyAttackState::PlayAttackAnimation()
        {
            /* 攻撃の種類をセットする。*/
            m_currentAttackType = AttackType::HeavyAttack;

            /* 再生するアニメーションを設定する。*/
            m_player->PlayWeaponAnimation(AttackType::HeavyAttack);
        }


        void PlayerHeavyAttackState::OnEnterAttack()
        {
            /* 銃の場合。*/
            if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
                m_player->SetWeaponRotationAngle(Vector3::Front, -90.0f);

            /* 当たり判定を付与。*/
            m_player->GetWeaponHitDetection().Enable();
        }


        bool PlayerHeavyAttackState::OnUpdateAttack()
        {
            /* アニメーション終了時の安全な待機状態遷移（銃は少し早いタイミング） */
            if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
            {
                if (!m_player->IsPlayAnimation()) {
                    m_stateMachine->ChangeState(new PlayerIdleState());
                    return true;
                }
            }
            else
            {
                if (m_attackTimer > 10 && !m_player->IsPlayAnimation()) {
                    m_stateMachine->ChangeState(new PlayerIdleState());
					return true;
                }
            }

            return false;
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