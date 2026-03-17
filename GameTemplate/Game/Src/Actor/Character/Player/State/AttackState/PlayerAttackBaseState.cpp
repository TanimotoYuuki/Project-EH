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


			if (g_pad[0]->IsTrigger(enButtonB))
			{
				/* 
				 *タイマーを加算する。
				 * Bボタンを押すごとにタイマーを加算し、当てはまるなら連続攻撃に繋げる。
				 */
				m_rushCount++;
			}

			if (m_attackTimer > 5 && !m_player->IsPlayAnimation())
			{
				/* Idle状態へ遷移。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
			}
		}


		void PlayerAttackBaseState::Exit()
		{
			/* Stateを抜ける際の処理。*/
			/* 効果音とかエフェクトとかの再生をストップさせる*/
		}
	}
}
