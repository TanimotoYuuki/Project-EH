#include "stdafx.h"
#include "NPCIdleState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
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
			if (m_body == nullptr || m_brain == nullptr)
				return;

			m_helpTarget = m_brain->GetHelpTarget();

			if (m_helpTarget != nullptr &&
				m_helpTarget != m_body &&
				(m_helpTarget->IsDeath() || m_helpTarget->GetCharacterStatus().hp.currentHP <= 0))
			{
				if (m_vInput != nullptr)
					m_vInput->Reset();

				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCHelpState(m_helpTarget));

				return;
			}

			auto target = m_brain->SearchTarget();
			if (target)
			{
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());

				return;
			}
		}
	}
}