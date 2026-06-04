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

			/* キャッシュの取得。*/
			m_body = m_brain->GetBody();
			/* 仮想入力の取得。*/
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* 入力のリセット。*/
			if (m_vInput)
				m_vInput->Reset();
		}


		void NPCIdleState::Update()
		{
			/* 救助要請がないかチェック。*/
			if (m_body == nullptr || m_brain == nullptr)
				return;

			/* 救助要請の確認。*/
			m_helpTarget = m_brain->GetHelpTarget();

			/* 救助要請がある場合、要請対象が死亡しているか、HPが0以下であるかを確認。*/
			if (m_helpTarget != nullptr &&
				m_helpTarget != m_body &&
				(m_helpTarget->IsDeath() || m_helpTarget->GetCharacterStatus().hp.currentHP <= 0))
			{
				/* 入力のリセット。*/
				if (m_vInput != nullptr)
					m_vInput->Reset();

				/* 救助要請がある場合、要請対象が死亡しているか、HPが0以下である場合は救助状態に遷移する。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCHelpState(m_helpTarget));

				return;
			}

			/* 目標の探索。*/
			auto target = m_brain->SearchTarget();
			if (target)
			{
				/* 入力のリセット。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());

				return;
			}
		}
	}
}