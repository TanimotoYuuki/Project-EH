#include "stdafx.h"
#include "NPCHelpState.h"

#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/NPC/Movement/NPCMovementController.h"
#include "Src/Utilty/ResourceUtility.h"
#include "Src/Sound/SoundLister.h"

namespace
{
	const auto MOVE_DEAD_ZONE = 0.001f;  //! 移動停止とみなす距離。
	const auto INPUT_HOLD_FRAME = 3;     //! 救助入力を押し続けるフレーム数。
	const auto VOLUME = 1.0f;            //! 救助 SE の音量。
	const auto LIFE_TIME = 2.0f;         //! 救助 SE の寿命。
}

namespace nsApp
{
	namespace nsState
	{
		NPCHelpState::NPCHelpState(nsActor::Player* helpTarget) : m_helpTarget(helpTarget)
		{
		}


		void NPCHelpState::Enter()
		{
			m_brain = static_cast<NPCBrain*>(m_owner);
			if (m_brain == nullptr)
				return;

			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			if (m_vInput != nullptr)
				m_vInput->Reset();

			m_hasRequestedHelp = false;

			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();
		}


		void NPCHelpState::Update()
		{
			if (m_brain == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 毎フレームタグで上書きしない。ターゲット未設定時だけ Brain から取得。*/
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();

			if (!IsValidHelpTarget())
			{
				if (m_vInput != nullptr)
					m_vInput->Reset();

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

			if (m_distance <= MOVE_DEAD_ZONE)
			{
				NPCMovementController::Stop(m_vInput);
				return;
			}

			/* 危険時はガードしながら接近。*/
			if (m_brain != nullptr && m_brain->ShouldGuard())
				m_vInput->RequestButton(enButtonLB2, 3);

			/* 救助対象へ走って近づく。*/
			const NPCMovementIntent intent = NPCMovementController::MakeMoveIntent(m_difference, true);
			NPCMovementController::Apply(m_vInput, intent);
		}


		void NPCHelpState::ExecuteHelp()
		{
			if (m_helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 救助中は移動を止める。*/
			NPCMovementController::Stop(m_vInput);

			StartHelpSE();

			/* 既に誰かが救助中なら上書きしない。*/
			if (m_helpTarget->GetRescueStatusManager().IsBeingHelped())
				return;

			/* 救助中もガードを維持（LB2 長押し）。*/
			m_vInput->RequestButton(enButtonLB2, INPUT_HOLD_FRAME);

			/* Player 側を直接救助ステートへ（攻撃中でも開始できる）。*/
			m_body->TryBeginHelpToTarget(m_helpTarget);
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

			auto* se = FindGO<nsSound::SoundLister>("SoundManager");
			if (se == nullptr)
				return;

			m_helpSE = se->GetSEList().PlaySE(nsSound::SE_ID::Rescue, VOLUME, true, LIFE_TIME);
		}


		void NPCHelpState::StopHelpSE()
		{
			if (m_helpSE == nullptr)
				return;

			auto* se = FindGO<nsSound::SoundLister>("SoundManager");
			if (se == nullptr)
			{
				m_helpSE = nullptr;
				return;
			}

			se->GetSEList().StopSE(m_helpSE);
		}
	}
}