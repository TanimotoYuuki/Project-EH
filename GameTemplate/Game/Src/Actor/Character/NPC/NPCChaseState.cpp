#include "stdafx.h"
#include "NPCChaseState.h"
#include "Src/Actor/Character/NPC/NPCIdleState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/NPC/NPCAttackState.h"

namespace nsApp
{
	namespace nsState
	{
		void NPCChaseState::Enter()
		{
			m_brain = static_cast<NPCBrain*>(m_owner);
		}


		void NPCChaseState::Update()
		{
			auto target = m_brain->SearchTarget();
			auto body = m_brain->GetBody();

			/* ターゲットを見失ったら待機ステートに戻る。*/
			if (target == nullptr || body == nullptr)
			{
				m_stateMachine->ChangeState(new NPCIdleState());
				return;
			}

			/* ターゲットとの距離を計算する。*/
			m_myPosition = body->GetPosition();
			m_targetPosition = target->GetPosition();
			m_difference = m_targetPosition - m_myPosition;
			m_distance = m_difference.Length();

			if (m_distance > 80.0f)
			{
				/* まだ遠い：敵の方向にスティックを倒して走る */
				m_difference.Normalize();
				body->GetInputClass().SetVirtualController(m_difference.x, m_difference.z);
			}
			else
			{
				/* 攻撃範囲（近距離）：立ち止まる */
				body->GetInputClass().SetVirtualController(0.0f, 0.0f);

				/* ターゲットが近づいてきたら攻撃ステートに遷移。*/
				m_stateMachine->ChangeState(new NPCAttackState());
			}
		}
	}
}