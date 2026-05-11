#include "stdafx.h"
#include "NPCAttackBaseState.h"

namespace
{
	const auto VIRTUAL_INPUT_VALUE_RESET = 0.0f;	//! 仮想入力のリセット値。
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f;     //! クリッピングを防止する距離。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCAttackBaseState::Enter()
		{
			/* キャスト。*/
			m_npcBrain = static_cast<NPCBrain*>(m_owner);

			/* NPCの体を取得。*/
			m_getBody = m_npcBrain->GetBody();

			/* 入力情報を取得。*/
			m_npcInput = &m_getBody->GetInputClass();

			/* タイマーを初期化。*/
			SetAttackTimer(0);
		}


		void NPCAttackBaseState::Exit()
		{
			if (m_npcInput)
			{
				/* 攻撃用仮想コントローラーを取得(Bボタン)。*/
				m_npcInput->SetVirtualButtonB(false);

				/* 仮想コントローラー情報をリセット。*/
				m_npcInput->SetVirtualController(VIRTUAL_INPUT_VALUE_RESET, VIRTUAL_INPUT_VALUE_RESET);
			}
		}


		void NPCAttackBaseState::PreventClipping(nsActor::ICharacter* target)
		{
			if (m_distance >= CLIPPING_LIMIT_DISTANCE || m_distance <= 0.0f)
				return;

			m_pushBakeDirection = m_getBody->GetPosition() - target->GetPosition();
			m_pushBakeDirection.y = 0.0f;
			m_pushBakeDirection.Normalize();

			m_currentPosition = m_getBody->GetPosition();
			m_currentPosition.x += m_pushBakeDirection.x * (CLIPPING_LIMIT_DISTANCE - m_distance);
			m_currentPosition.z += m_pushBakeDirection.z * (CLIPPING_LIMIT_DISTANCE - m_distance);

			m_getBody->GetCharacterController().SetPosition(m_currentPosition);
			m_getBody->SetPosition(m_currentPosition);
		}


		void NPCAttackBaseState::UpdateFacingDirection()
		{
			if (m_isAttacking) {
				m_getBody->SetAngle(m_diff.x > 0.0f ? 90.0f : -90.0f);
				m_getBody->SetForwardVector(m_diff.x > 0.0f ? Vector3::Right : Vector3::Left);
			}
		}


		void NPCAttackBaseState::ResetVirtualInputs() 
		{
			/* 入力情報がなければリターン。*/
			if (!m_npcInput)
				return;

			/* 仮想コントローラー情報をリセット。*/
			m_npcInput->SetVirtualButtonA(false);
			m_npcInput->SetVirtualButtonB(false);
			m_npcInput->SetVirtualButtonX(false);
			m_npcInput->SetVirtualButtonLB1(false);
			m_npcInput->SetVirtualButtonLB2(false);
			m_npcInput->SetVirtualButtonRB1(false);
			m_npcInput->SetVirtualButtonRT(false);
		}
	}
}