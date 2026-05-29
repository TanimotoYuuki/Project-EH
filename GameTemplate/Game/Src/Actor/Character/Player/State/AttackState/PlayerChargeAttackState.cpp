#include "stdafx.h"
#include "PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargingState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerHeelMagicState.h"

#include "Src/Actor/Gun/Factory/BulletFactory.h"

#define FIRE_FRAME 15

#define PLAYER_PLAY_ANIMATION m_player->PlayWeaponAnimation
#define CURRENT_WEAPON m_currentAttackType = AttackType

namespace nsApp
{
	namespace nsState
	{
		void PlayerChargeAttackState::PlayAttackAnimation()
		{
			/* 武器の種類によってアニメーションを変化させる。*/
			if (m_player->GetCurrentWeapon() == WeaponType::Hammer)
			{
				m_currentAttackType = AttackType::HeavyAttack;
				m_player->PlayWeaponAnimation(AttackType::ChargeAttack);
			}

			else
			{
				m_currentAttackType = AttackType::ChargeAttack;
				m_player->PlayWeaponAnimation(AttackType::ChargeAttack);
			}
		}


		void PlayerChargeAttackState::OnAttackTick()
		{
			/* 1フレーム目では当たり判定を有効化させる。*/
			if (m_attackTimer == 1)
				m_player->GetWeaponHitDetection().Enable();

			/* 弾丸を生成。*/
			if (m_attackTimer == FIRE_FRAME && m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				FireChargeBullet();
		}


		void PlayerChargeAttackState::FireChargeBullet()
		{
			/* 武器の当たり判定を取得する。*/
			m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();

			/* 前方向のベクトルを取得する。*/
			m_forwardDirection = m_player->GetForwardVector();

			/* 生成する弾丸の種類を設定する。*/
			ConstructAndTransmitBulletRequest(BulletType::enCharge);
		}
	}
}