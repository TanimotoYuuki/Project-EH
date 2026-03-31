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

			/* 攻撃のタイプをセット。*/
			m_currentAttackType = AttackType::ChargeAttack;

			/* アニメーションを再生させる。*/
			m_player->PlayWeaponAnimation(AttackType::ChargeAttack);

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