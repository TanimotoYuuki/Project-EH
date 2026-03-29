#include "stdafx.h"
#include "PlayerGuardState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerGuardState::Enter()
		{
			/* アニメーションをキャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Guard);
		}

		void PlayerGuardState::Update()
		{
			if (!m_player->GetInputClass().IsGuard())
				m_stateMachine->ChangeState(new PlayerIdleState());
		}


		void PlayerGuardState::Exit()
		{
		}


		bool PlayerGuardState::RequestID(uint8_t& id)
		{
			return false;
		}
	}
}