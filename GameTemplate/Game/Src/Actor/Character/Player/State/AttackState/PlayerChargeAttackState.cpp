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

			/* アニメーションを再生させる。*/
			m_player->PlayWeaponAnimation(AttackType::ChargeAttack);
		}


		void PlayerChargeAttackState::Update()
		{
			/* 親クラスを更新。*/
			PlayerAttackBaseState::Update();
		}
	}
}