#include "stdafx.h"
#include "PlayerReBoneState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerReBoneState::Enter()
		{
			m_player = static_cast<nsActor::Player*>(m_owner);
			m_isHelpStarted = false;
			m_isHelpCompleted = false;
			m_shouldReturnIdle = false;

			if (m_player == nullptr)
				return;

			/* �~���Ώۂ���������B*/
			ResolveTargetCharacter();

			/* �~���Ώۂ����Ȃ��ꍇ�A���̃t���[���Ŗ߂��B*/
			if (!IsValidTargetCharacter())
			{
				m_shouldReturnIdle = true;
				return;
			}

			/* �~���i�s���J�n����B*/
			BeginHelpProgress();

			/* �~�����͓��͂��~�߂�B*/
			m_player->SetInputEnable(false);

			/* �~���A�j���[�V�������Đ��B*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Help);

			/* �~���s��������UI�֒ʒm����B*/
			auto* commentary = FindGO<nsUI::CommentaryUIManager>("CommentaryUIManager");
			if (commentary != nullptr)
				commentary->AddActionMessage(m_player->GetCurrentWeapon(), L"Help");
		}


		void PlayerReBoneState::Update()
		{
			/* �~���J�n�O�A�܂��͋~���Ώۂ������ɂȂ����ꍇ�͑ҋ@�X�e�[�g�ɖ߂��B*/
			if (m_shouldReturnIdle)
			{
				ChangeIdleState();
				return;
			}

			/* �~���J�n�O�͑Ώۂ̗L�������m�F���Ȃ��B*/
			if (m_player == nullptr || m_targetPlayer == nullptr)
			{
				ChangeIdleState();
				return;
			}

			/* �~���J�n��͑Ώۂ̗L��������Ɋm�F����B*/
			if (!IsValidTargetCharacter())
			{
				ChangeIdleState();
				return;
			}

			/* �~���J�n��͋~���i�s���X�V����B*/
			auto& rescueStatus = m_targetPlayer->GetRescueStatusManager();

			/* �~���i�s��RescueStatusManager�ɔC����B*/
			if (rescueStatus.UpdateHelpProgress())
			{
				CompleteHelp();
				return;
			}

			/* �K�v�~�����Ԃ��A�j���[�V������蒷���ꍇ�́A�����ڂ����Đ��������B*/
			if (!m_player->IsPlayAnimation())
				m_player->PlayBasicAnimation(CharacterBasicAnimationList::Help);
		}


		void PlayerReBoneState::Exit()
		{
			/* �~�����͓��͂��~�߂�B*/
			if (m_player != nullptr)
				m_player->SetInputEnable(true);

			/* �~�����������Ă��Ȃ��ꍇ�́A�~�����L�����Z������B*/
			if (!m_isHelpCompleted && m_targetPlayer != nullptr && IsValidTargetCharacter())
				m_targetPlayer->GetRescueStatusManager().CancelHelp();

			m_player = nullptr;
			m_targetPlayer = nullptr;
			m_isHelpStarted = false;
			m_isHelpCompleted = false;
			m_shouldReturnIdle = false;
		}


		void PlayerReBoneState::ResolveTargetCharacter()
		{
			if (m_player == nullptr)
				return;

			/* SetTargetCharacter�ŊO����w��ς݂Ȃ�A������ŗD�悷��B*/
			if (m_targetPlayer != nullptr)
				return;

			/* NPC�~���̏ꍇ�́ANPCHelpState/NPCBrain�����߂��Ώۂ�D�悷��B*/
			if (m_player->GetBrain() != nullptr)
				m_targetPlayer = m_player->GetBrain()->GetHelpTarget();

			/* �v���C���[����̋~���A�܂���NPC���ɑΏۂ������ꍇ�͎��͌�������B*/
			if (m_targetPlayer == nullptr)
				m_targetPlayer = m_player->SearchCharacter();
		}


		bool PlayerReBoneState::IsValidTargetCharacter() const
		{
			/* �Ώۂ����݂��Ȃ��A�܂��͎������g��Ώۂɂ��Ă���ꍇ�͖����B*/
			if (m_player == nullptr || m_targetPlayer == nullptr)
				return false;

			/* �������g��Ώۂɂ��邱�Ƃ͂ł��Ȃ��B*/
			if (m_targetPlayer == m_player)
				return false;

			/* �Ώۂ����S���Ă���A�܂���HP��0�ȉ��̏ꍇ�͖����B*/
			return m_targetPlayer->IsDeath() ||
				m_targetPlayer->GetCharacterStatus().hp.currentHP <= 0;
		}


		void PlayerReBoneState::BeginHelpProgress()
		{
			/* �~���J�n�O�A�܂��͋~���Ώۂ������ɂȂ����ꍇ�͋~�����J�n���Ȃ��B*/
			if (m_player == nullptr || m_targetPlayer == nullptr || m_isHelpStarted)
				return;

			/* �~���J�n�B*/
			m_targetPlayer->GetRescueStatusManager().BeginHelp(m_player->GetCurrentWeapon());
			m_isHelpStarted = true;
		}


		void PlayerReBoneState::CompleteHelp()
		{
			if (m_targetPlayer != nullptr)
			{
				/* �~�������B*/
				m_isHelpCompleted = true;

				/* �~���Ώۂ𕜊�������B*/
				m_targetPlayer->ReceiveHelp();
				m_targetPlayer->GetRescueStatusManager().ResetRescueStatus();
			}

			/* �ҋ@�X�e�[�g�ɖ߂��B*/
			ChangeIdleState();
		}


		void PlayerReBoneState::ChangeIdleState()
		{
			/* �~���J�n�O�A�܂��͋~���Ώۂ������ɂȂ����ꍇ�͑ҋ@�X�e�[�g�ɖ߂��B*/
			if (m_stateMachine != nullptr)
				m_stateMachine->ChangeState(new PlayerIdleState());
		}
	}
}
