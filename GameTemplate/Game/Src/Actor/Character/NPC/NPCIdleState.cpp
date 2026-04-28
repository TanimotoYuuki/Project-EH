#include "stdafx.h"
#include "NPCIdleState.h"
#include "Src/Actor/Character/NPC/NPCChaseState.h" 
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
			auto body = m_brain->GetBody();
			if (body)
			{
				body->GetInputClass();
			}
		}


		void NPCIdleState::Update()
		{
			/* ターゲットを見つけたら、追従ステートに遷移。*/
			auto target = m_brain->SearchTarget();
			if (target)
				m_stateMachine->ChangeState(new NPCChaseState());
		}
	}
}