#include "stdafx.h"
#include "PlayerGetUpState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerGetUpState::Enter()
		{
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 起き上がりアニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Hit_UP);
		}


		void PlayerGetUpState::Update()
		{
			/* アニメーションが終わったら待機状態へ。*/
			if (!m_player->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
			}
		}
	}
}