#include "stdafx.h"
#include "PlayerReBoneState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerReBoneState::Enter()
		{
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 助ける対象のPlayer/NPCを探索する。*/
			m_targetPlayer = m_player->SearchCharacter();

			// 起き上がりアニメーションを再生
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Help);
		}

		void PlayerReBoneState::Update()
		{
			// 起き上がりアニメーションが終わったら待機状態へ
			if (!m_player->IsPlayAnimation())
			{
				if (m_targetPlayer != nullptr)
					m_targetPlayer->ReceiveHelp();

				m_stateMachine->ChangeState(new PlayerIdleState());
			}
		}
	}
}