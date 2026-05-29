#include "stdafx.h"
#include "NPCHelpState.h"

#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "ResourceUtility.h"
#include "Src/Sound/SoundLister.h"

namespace nsApp
{
	namespace nsState
	{
		NPCHelpState::NPCHelpState(nsActor::Player* helpTarget)
			: m_helpTarget(helpTarget)
		{
		}


		void NPCHelpState::Enter()
		{
			/* キャスト。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			if (m_brain == nullptr)
				return;

			/* Brainから必要なコンポーネントを取得する。*/
			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			if (m_vInput != nullptr)
				m_vInput->Reset();

			m_hasRequestedHelp = false;

			/* コンストラクタで渡された救助対象を使う。*/ 
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();
		}


		void NPCHelpState::Update()
		{
			if (m_brain == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 毎フレームタグ検索で上書きしない。*/ 
			/* ターゲットが未設定のときだけ、Brainが見つけた救助対象を使う。*/ 
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();

			/* 救助対象が有効でない場合、入力をリセットして追いかけ状態に遷移する。*/
			if (!IsValidHelpTarget())
			{
				/* 入力情報をリセットする。*/
				m_vInput->Reset();

				/* 追いかけ状態に遷移する。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());

				return;
			}

			m_difference = m_helpTarget->GetPosition() - m_body->GetPosition();
			m_difference.y = 0.0f;

			m_distance = m_difference.Length();

			if (m_distance > m_helpRange)
			{
				MoveToHelpTarget();
				return;
			}

			ExecuteHelp();
		}


		void NPCHelpState::Exit()
		{
			if (m_vInput != nullptr)
				m_vInput->Reset();

			if (m_helpTarget != nullptr && m_helpTarget != m_body)
				m_helpTarget->GetRescueStatusManager().SetBeingHelped(false);

			/* SEを停止する。*/
			StopHelpSE();

			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
			m_helpTarget = nullptr;
			m_difference = Vector3::Zero;
			m_distance = 0.0f;
		}


		void NPCHelpState::MoveToHelpTarget()
		{
			if (m_vInput == nullptr)
				return;

			if (m_distance <= 0.001f)
			{
				m_vInput->SetLStick(0.0f, 0.0f);
				m_vInput->SetButton(enButtonLB1, false);
				return;
			}

			m_difference.Normalize();
			m_vInput->SetLStick(m_difference.x, m_difference.z);
			m_vInput->SetButton(enButtonLB1, true);
		}


		void NPCHelpState::ExecuteHelp()
		{
			/* 救助対象が有効でない場合、入力をリセットして追いかけ状態に遷移する。*/
			if (m_helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 入力情報をリセットする。*/
			m_vInput->SetLStick(0.0f, 0.0f);

			/* SEの再生。*/
			StartHelpSE();

			/* 走り解除。*/
			m_vInput->SetButton(enButtonLB1, false);

			/* 救助要求は一度だけ行う。*/ 
			if (!m_helpTarget->GetRescueStatusManager().IsBeingHelped())
			{
				/* Help入力。*/
				m_vInput->RequestButton(enButtonY, 3);
			}
		}


		bool NPCHelpState::IsValidHelpTarget() const
		{
			if (m_helpTarget == nullptr)
				return false;

			if (m_helpTarget == m_body)
				return false;

			return m_helpTarget->IsDeath() || m_helpTarget->GetCharacterStatus().hp.currentHP <= 0;
		}


		void NPCHelpState::StartHelpSE()
		{
			if (m_helpSE != nullptr)
				return;
			
			/* 音源管理クラスを探索する。*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");

			if (se == nullptr)
				return;

			/* SEを再生する。*/
			m_helpSE = se->GetSEList().PlaySE(nsSound::SE_ID::Rescue, 1.0f, true, 0.0f);
		}


		void NPCHelpState::StopHelpSE()
		{
			if (m_helpSE == nullptr)
				return;

			/* 音源管理クラスを探索する。*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");

			if (se == nullptr)
				return;

			if (se == nullptr)
			{
				m_helpSE = nullptr;
				return;
			}

			se->GetSEList().StopSE(m_helpSE);
		}
	}
}

