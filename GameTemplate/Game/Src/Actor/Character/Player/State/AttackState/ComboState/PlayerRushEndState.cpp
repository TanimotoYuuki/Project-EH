#include "stdafx.h"
#include "PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerRushEndState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションの再生。*/
			m_player->PlayWeaponAnimation(AttackType::RushAttack_End);
		}

		void PlayerRushEndState::Update()
		{
			/* タイマーを加算。*/
			m_attackTimer++;

			/* アニメーションの再生が終わったらタメ攻撃状態に遷移。*/ 
			if (!m_player->IsPlayAnimation())
				m_stateMachine->ChangeState(new PlayerChargeAttackState());
		}
	}
}