#include "stdafx.h"
#include "PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargingState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerHeelMagicState.h"


#define PLAYER_PLAY_ANIMATION m_player->PlayWeaponAnimation
#define CURRENT_WEAPON m_currentAttackType = AttackType

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
				CURRENT_WEAPON::HeavyAttack;
				PLAYER_PLAY_ANIMATION(AttackType::ChargeAttack);
			}

			else
			{
				CURRENT_WEAPON::ChargeAttack;
				PLAYER_PLAY_ANIMATION(AttackType::ChargeAttack);
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