#include "stdafx.h"
#include "PlayerHeavyAttackState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

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

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerHeavyAttackState::Update()
		{
			/* 親クラスの更新。*/
			PlayerAttackBaseState::Update();

			/* アニメーションの終了判定。*/
			if (m_attackTimer > 10 && !m_player->IsPlayAnimation())
				m_stateMachine->ChangeState(new PlayerIdleState);
		}
	}
}