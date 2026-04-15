#include "stdafx.h"
#include "PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerChargeAttackState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 武器の種類によってチャージ後の攻撃のアニメーションを変化させる。*/
			if (m_player->GetCurrentWeapon() == WeaponType::Hammer)
			{
				m_currentAttackType == AttackType::HeavyAttack;
				m_player->PlayWeaponAnimation(AttackType::ChargeAttack);
			}

			else
			{
				m_currentAttackType == AttackType::ChargeAttack;
				m_player->PlayWeaponAnimation(AttackType::ChargeAttack);
			}

			/* 当たり判定を有効にする。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerChargeAttackState::Update()
		{

			/* 親クラスを更新。*/
			PlayerAttackBaseState::Update();
		}
	}
}