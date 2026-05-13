#include "stdafx.h"
#include "NPCAttackBaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace
{
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

			/* NPCBrainクラスからAdapterを取得。*/
			m_virtualInput = m_npcBrain->GetVirtualInputAdapter();

			/* タイマーを初期化。*/
			SetAttackTimer(0);
		}


		void NPCAttackBaseState::Exit()
		{
			if (m_virtualInput)
				m_virtualInput->Reset();
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
			if (!m_virtualInput)
				return;

			/* 入力情報のリセット。*/
			m_virtualInput->Reset();
		}
	}
}