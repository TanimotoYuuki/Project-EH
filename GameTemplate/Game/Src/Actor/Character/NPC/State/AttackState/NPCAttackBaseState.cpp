#include "stdafx.h"
#include "NPCAttackBaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
#include "Src/Actor/Character/NPC/Component/NPCCombatHelper.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCEvadeState.h"

namespace
{
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f; //! クリッピングを防止する距離。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCAttackBaseState::Enter()
		{
			/* キャスト。*/
			m_npcBrain = static_cast<NPCBrain *>(m_owner);

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
			/* 攻撃インターバルを開始。*/
			if (m_npcBrain != nullptr)
			{
				/* NPCBrainクラスから攻撃インターバル開始処理を呼び出す。*/
				m_npcBrain->StartAttackInterval();

				/* VirtualInputAdapterが存在する場合は、入力をリセットする。*/
				auto *virtualInput = m_npcBrain->GetVirtualInputAdapter();
				if (virtualInput != nullptr)
					virtualInput->Reset();
			}

			m_virtualInput = nullptr;
			m_getBody = nullptr;
			m_npcBrain = nullptr;
		}


		void NPCAttackBaseState::ComputeDistance(nsActor::ICharacter *targetObject)
		{
			if (m_getBody == nullptr || targetObject == nullptr)
			{
				m_distance = 0.0f;
				m_diff = Vector3::Zero;
				return;
			}

			/* ヘルパークラスから距離計算処理を呼び出す。*/
			m_distance = NPCCombatHelper::ComputeDistance(m_getBody->GetPosition(), targetObject->GetPosition(), m_diff);
		}


		void NPCAttackBaseState::PreventClipping(nsActor::ICharacter *target)
		{
			/* Playerクラスが居ないなら処理を止める。*/
			if (m_getBody == nullptr || target == nullptr)
				return;

			/* ヘルパークラスからクリッピング防止処理を呼び出す。*/
			NPCCombatHelper::PreventClipping(m_getBody, target, m_distance, 40.0f);
		}


		void NPCAttackBaseState::UpdateFacingDirection()
		{
			/* Playerクラスが居ないなら処理を止める。*/
			if (m_getBody == nullptr)
				return;

			/* ヘルパークラスから向き更新処理を呼び出す。*/
			NPCCombatHelper::UpdateFacing(m_getBody, m_diff, m_isAttacking);
		}


		void NPCAttackBaseState::ResetVirtualInputs()
		{
			/* 入力情報が無いなら処理を止める。*/
			if (!m_virtualInput)
				return;

			/* VirtualInputAdapterクラスのResetメソッドを呼び出す。*/
			m_virtualInput->Reset();
		}


		bool NPCAttackBaseState::CheckHelpTransition()
		{
			/* NPCBrainクラスが存在しない場合は、以降の処理を行わない。*/
			if (m_npcBrain == nullptr || m_stateMachine == nullptr)
				return false;

			/* ヘルプ対象を取得。*/
			auto *helpTarget = m_npcBrain->GetHelpTarget();
			if (helpTarget == nullptr)
				return false;

			/* ヘルプ対象が死亡していない、またはHPが0より大きい場合は、ヘルプ状態に遷移しない。*/
			if (!helpTarget->IsDeath() && helpTarget->GetCharacterStatus().hp.currentHP > 0)
				return false;

			/* この NPC が救助役かどうかを判定する。*/
			if (!m_npcBrain->ShouldRespondToHelp())
				return false;

			/* ヘルプ状態に遷移する。*/
			m_stateMachine->ChangeState(new NPCHelpState(helpTarget));

			return true;
		}


		bool NPCAttackBaseState::CheckEvadeTransition()
		{
			if (m_npcBrain == nullptr || m_stateMachine == nullptr)
				return false;

			if (!m_npcBrain->ShouldEvade())
				return false;

			/* 攻撃モーション中は致命圏以外 Evade しない。*/
			if (m_isAttacking && !m_npcBrain->IsDangerous())
				return false;

			m_stateMachine->ChangeState(new NPCEvadeState());
			return true;
		}
	}
}