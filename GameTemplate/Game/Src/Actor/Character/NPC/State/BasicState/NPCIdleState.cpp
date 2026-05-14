#include "stdafx.h"
#include "NPCIdleState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		void NPCIdleState::Enter()
		{
			/* キャスト。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			m_body = m_brain->GetBody();

			m_vInput = m_brain->GetVirtualInputAdapter();
			if (m_vInput)
				m_vInput->Reset();
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