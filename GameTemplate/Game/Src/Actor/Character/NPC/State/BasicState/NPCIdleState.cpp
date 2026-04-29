#include "stdafx.h"
#include "NPCIdleState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h" 
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		void NPCIdleState::Enter()
		{
			/* キャスト。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			/* 仮想コントローラーの入力をリセットする。*/
			m_body = m_brain->GetBody();
			if (m_body)
			{
				m_body->GetInputClass().SetVirtualController(0.0f, 0.0f);
				m_body->GetInputClass().SetVirtualButtonB(false);
				m_body->GetInputClass().SetVirtualButtonY(false);
			}
		}


		void NPCIdleState::Update()
		{
			/* 早期リターン。*/
			if (!m_body)
				return;

			/* ターゲットを見つけたら、追従ステートに遷移。*/
			auto target = m_brain->SearchTarget();
			if (target)
				m_stateMachine->ChangeState(new NPCChaseState());
		}
	}
}