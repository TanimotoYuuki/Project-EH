#include "stdafx.h"
#include "NPCAttackBaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
#include "Src/Actor/Character/NPC/Component/NPCCombatHelper.h"

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

			/* キャストに失敗した場合は、以降の処理を行わない。*/	
			if (m_npcBrain == nullptr)
			{
				m_getBody = nullptr;
				m_virtualInput = nullptr;
				return;
			}

			/* NPCの体を取得。*/
			m_getBody = m_npcBrain->GetBody();

			/* NPCBrainクラスからAdapterを取得。*/
			m_virtualInput = m_npcBrain->GetVirtualInputAdapter();

			/* タイマーを初期化。*/
			SetAttackTimer(0);
		}


		void NPCAttackBaseState::Exit()
		{
			if (m_npcBrain != nullptr)
			{
				m_npcBrain->StartAttackInterval();

				auto* virtualInput = m_npcBrain->GetVirtualInputAdapter();
				if (virtualInput != nullptr)
					virtualInput->Reset();
			}

			m_virtualInput = nullptr;
			m_getBody = nullptr;
			m_npcBrain = nullptr;
		}


		void NPCAttackBaseState::ComputeDistance(nsActor::ICharacter* targetObject)
		{
			if (m_getBody == nullptr || targetObject == nullptr)
			{
				m_distance = 0.0f;
				m_diff = Vector3::Zero;
				return;
			}

			/* ヘルパークラスから距離計算処理を呼び出す。*/
			m_distance =  NPCCombatHelper::ComputeDistance(m_getBody->GetPosition(), targetObject->GetPosition(), m_diff);
		}


		void NPCAttackBaseState::PreventClipping(nsActor::ICharacter* target)
		{
			if (m_getBody == nullptr || target == nullptr)
				return;

			/* ヘルパークラスからクリッピング防止処理を呼び出す。*/
			NPCCombatHelper::PreventClipping(m_getBody, target, m_distance, 40.0f);
		}


		void NPCAttackBaseState::UpdateFacingDirection()
		{
			if (m_getBody == nullptr)
				return;

			/* ヘルパークラスから向き更新処理を呼び出す。*/
			NPCCombatHelper::UpdateFacing(m_getBody, m_diff, m_isAttacking);
		}


		void NPCAttackBaseState::ResetVirtualInputs() 
		{
			if (!m_virtualInput)
				return;

			m_virtualInput->Reset();
		}


		bool NPCAttackBaseState::CheckHelpTransition()
		{
			if (m_npcBrain == nullptr || m_stateMachine == nullptr)
				return false;

			auto* helpTarget = m_npcBrain->GetHelpTarget();
			if (helpTarget == nullptr)
				return false;

			if (helpTarget == m_getBody)
				return false;

			if (!helpTarget->IsDeath() && helpTarget->GetCharacterStatus().hp.currentHP > 0)
				return false;

			m_stateMachine->ChangeState(new NPCHelpState(helpTarget));

			return true;
		}
	}
}