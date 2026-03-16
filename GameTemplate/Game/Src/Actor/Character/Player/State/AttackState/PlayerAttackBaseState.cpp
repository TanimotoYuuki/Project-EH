#include "stdafx.h"
#include "PlayerAttackBaseState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerAttackBaseState::Enter()
		{
			/* 攻撃の種類ごとにキャストを行う。*/
			m_player = static_cast<nsActor::Player*>(m_owner);
		}


		void PlayerAttackBaseState::Update()
		{
			/* タイマーを加算する。*/
			m_attackTimer++;

			if (m_attackTimer > 5)
			{
				/* 攻撃が終了後、Idle状態へ戻るように。*/
				if (!m_player->IsPlayAnimation())
				{
					/* Idle状態へ遷移。*/
					m_stateMachine->ChangeState(new PlayerIdleState());
				}
			}
		}


		void PlayerAttackBaseState::Exit()
		{
			/* Stateを抜ける際の処理。*/
			/* 効果音とかエフェクトとかの再生をストップさせる*/
		}
	}
}
