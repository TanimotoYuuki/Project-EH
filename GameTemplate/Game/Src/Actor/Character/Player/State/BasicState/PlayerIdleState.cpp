#include "stdafx.h"
#include "PlayerIdleState.h"

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
			const auto& inputClass = m_player->GetInputClass();

			/* 死亡判定。*/
			if (inputClass.IsDeath())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enDeath);
				return true;
			}

			/* ダメージ状態。*/
			if (inputClass.IsDamage())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enHit);
				return true;
			}

			/* ジャンプ状態。*/
			if (inputClass.IsJump())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enJump);
				return true;
			}

			/* 走り状態。*/
			if (inputClass.IsRun())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enRun);
				return true;
			}

			/* 歩き状態。*/
			if(inputClass.IsMove())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enWalk);
				return true;
			}

			return false;
		}
	}
}