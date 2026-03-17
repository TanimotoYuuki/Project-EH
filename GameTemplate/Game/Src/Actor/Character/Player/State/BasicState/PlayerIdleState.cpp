#include "stdafx.h"
#include "PlayerIdleState.h"
#include "Src/Actor/Character/Player/Component/StateTransitionDiagram.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"

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
			const auto& inputClass = m_player->GetInputClass();
			/* Bボタンが押されているか検出。*/
			if (inputClass.IsAttack())
			{
				/* 攻撃ボタンが押されたら通常攻撃に切り替える。*/
				m_stateMachine->ChangeState(new PlayerNormalAttackState());
				return;
			}
		}


		void PlayerIdleState::Exit()
		{

		}


		bool PlayerIdleState::RequestID(uint8_t& id)
		{
			return StateTransitionDiagram::CheckCommonTransition(m_player->GetInputClass(), id);
		}
	}
}