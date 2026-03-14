#include "stdafx.h"
#include "PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerIdleState::Enter() 
		{
			/* @TODO: 基本動作のIdleアニメーションを再生する。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* Playerのアニメーション再生処理をコール。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Idle);
		}


		void PlayerIdleState::Update()
		{

		}


		void PlayerIdleState::Exit()
		{

		}
	}
}