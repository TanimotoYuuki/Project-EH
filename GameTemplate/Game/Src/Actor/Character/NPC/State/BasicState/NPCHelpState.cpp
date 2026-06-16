#include "stdafx.h"
#include "NPCHelpState.h"

#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Utilty/ResourceUtility.h"
#include "Src/Sound/SoundLister.h"

namespace
{
	const auto INPUT_STICK_VALUE_X = 1.0f; //! �~���Ώۂɋ߂Â����߂̃X�e�B�b�N���͂�X�l�B
	const auto INPUT_STICK_VALUE_Y = 1.0f; //! �~���Ώۂɋ߂Â����߂̃X�e�B�b�N���͂�Y�l�B
											   
	const auto INPUT_HOLD_FRAME = 3;	   //! �~�����͂�����������t���[�����B
	const auto VOLUME = 1.0f;			   //! �~��SE�̉��ʁB
	const auto LIFE_TIME = 2.0f;		   //! �~��SE�̎����B
}

namespace nsApp
{
	namespace nsState
	{
		NPCHelpState::NPCHelpState(nsActor::Player* helpTarget) : m_helpTarget(helpTarget){}


		void NPCHelpState::Enter()
		{
			/* �L���X�g�B*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			if (m_brain == nullptr)
				return;

			/* Brain����K�v�ȃR���|�[�l���g���擾����B*/
			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* ���͏������Z�b�g����B*/
			if (m_vInput != nullptr)
				m_vInput->Reset();

			/* �~���v���͂܂��s���Ă��Ȃ��B*/
			m_hasRequestedHelp = false;

			/* �R���X�g���N�^�œn���ꂽ�~���Ώۂ��g���B*/ 
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();
		}


		void NPCHelpState::Update()
		{
			/* �~���Ώۂ��L���łȂ��ꍇ�A���͂����Z�b�g���Ēǂ�������ԂɑJ�ڂ���B*/
			if (m_brain == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* ���t���[���^�O�����ŏ㏑�����Ȃ��B*/ 
			/* �^�[�Q�b�g�����ݒ�̂Ƃ������ABrain���������~���Ώۂ��g���B*/ 
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();

			/* �~���Ώۂ��L���łȂ��ꍇ�A���͂����Z�b�g���Ēǂ�������ԂɑJ�ڂ���B*/
			if (!IsValidHelpTarget())
			{
				/* ���͏������Z�b�g����B*/
				m_vInput->Reset();

				/* �ǂ�������ԂɑJ�ڂ���B*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());

				return;
			}

			/* �~���ΏۂƂ̋������v�Z����B*/
			m_difference = m_helpTarget->GetPosition() - m_body->GetPosition();
			m_difference.y = 0.0f;

			/* �������v�Z����B*/
			m_distance = m_difference.Length();

			/* ���̋����ȏ㗣�ꂽ�ꍇ�A�~���Ώۂɋ߂Â��B*/
			if (m_distance > m_helpRange)
			{
				/* �߂Â��B*/
				MoveToHelpTarget();
				return;
			}

			/* �~���Ώۂɋ߂Â��Ă���ꍇ�AHelp���͂����s����B*/
			ExecuteHelp();
		}


		void NPCHelpState::Exit()
		{
			/* ���͏������Z�b�g����B*/
			if (m_vInput != nullptr)
				m_vInput->Reset();

			/* �~���Ώۂ̏�Ԃ����Z�b�g����B*/
			if (m_helpTarget != nullptr && m_helpTarget != m_body)
				m_helpTarget->GetRescueStatusManager().SetBeingHelped(false);

			/* SE���~����B*/
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
			/* �~���Ώۂ��L���łȂ��ꍇ�A���͂����Z�b�g���Ēǂ�������ԂɑJ�ڂ���B*/
			if (m_vInput == nullptr)
				return;

			/* ���̋����ȏ㗣�ꂽ�ꍇ�A�~���Ώۂɋ߂Â��B*/
			if (m_distance <= 0.001f)
			{
				/* �~���Ώۂɔ��ɋ߂��ꍇ�A�j���[�g�����ȃX�e�B�b�N���͂�ݒ肷��B*/
				m_vInput->SetLStick(INPUT_STICK_VALUE_X, INPUT_STICK_VALUE_Y);
				m_vInput->SetButton(enButtonLB1, false);
				return;
			}

			/* �~���Ώۂɋ߂Â����߂̃X�e�B�b�N���͂�ݒ肷��B*/
			m_difference.Normalize();
			m_vInput->SetLStick(m_difference.x, m_difference.z);
			m_vInput->SetButton(enButtonLB1, true);
		}


		void NPCHelpState::ExecuteHelp()
		{
			/* �~���Ώۂ��L���łȂ��ꍇ�A���͂����Z�b�g���Ēǂ�������ԂɑJ�ڂ���B*/
			if (m_helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* ���͏������Z�b�g����B*/
			m_vInput->SetLStick(INPUT_STICK_VALUE_X, INPUT_STICK_VALUE_Y);

			/* SE�̍Đ��B*/
			StartHelpSE();

			/* ��������B*/
			m_vInput->SetButton(enButtonLB1, false);

			/* �~���v���͈�x�����s���B*/ 
			if (!m_helpTarget->GetRescueStatusManager().IsBeingHelped())
			{
				/* Help���́B*/
				m_vInput->RequestButton(enButtonY, INPUT_HOLD_FRAME);
			}
		}


		bool NPCHelpState::IsValidHelpTarget() const
		{
			/* �~���Ώۂ��L���łȂ��ꍇ�A���͂����Z�b�g���Ēǂ�������ԂɑJ�ڂ���B*/
			if (m_helpTarget == nullptr)
				return false;

			/* �~���Ώۂ��������g�̏ꍇ�́A���S��Ԃ�HP0�ȉ��̏�Ԃ�L���ȋ~���ΏۂƂ���B*/
			if (m_helpTarget == m_body)
				return false;

			return m_helpTarget->IsDeath() || m_helpTarget->GetCharacterStatus().hp.currentHP <= 0;
		}


		void NPCHelpState::StartHelpSE()
		{
			/* ���ł�SE���Đ�����Ă���ꍇ�́A�ēx�Đ����Ȃ��B*/
			if (m_helpSE != nullptr)
				return;
			
			/* �����Ǘ��N���X��T������B*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");

			/* �����Ǘ��N���X��������Ȃ��ꍇ�͏����𒆒f����B*/
			if (se == nullptr)
				return;

			/* SE���Đ�����B*/
			m_helpSE = se->GetSEList().PlaySE(nsSound::SE_ID::Rescue, VOLUME, true, LIFE_TIME);
		}


		void NPCHelpState::StopHelpSE()
		{
			/* SE���Đ�����Ă��Ȃ��ꍇ�́A�����𒆒f����B*/
			if (m_helpSE == nullptr)
				return;

			/* �����Ǘ��N���X��T������B*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");

			/* �����Ǘ��N���X��������Ȃ��ꍇ�͏����𒆒f����B*/
			if (se == nullptr)
				return;

			if (se == nullptr)
			{
				m_helpSE = nullptr;
				return;
			}

			/* SE���~����B*/
			se->GetSEList().StopSE(m_helpSE);
		}
	}
}

