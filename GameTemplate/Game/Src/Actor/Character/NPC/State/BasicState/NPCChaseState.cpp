#include "stdafx.h"
#include "NPCChaseState.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Actor/Character/NPC/State/AttackState/NPCSwordAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCHammerAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCWandAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCTwinGunAttackState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
#include "Src/Actor/Character/NPC/Component/NPCCombatHelper.h"
#include "Src/Actor/Character/NPC/Component/NPCActionParameterTable.h"

namespace
{
	/* ���͒l�B*/
	const auto INPUT_STICK_X = 0.0f;           //! �X�e�B�b�N��X���͒l�B
	const auto INPUT_STICK_Y = 0.0f;           //! �X�e�B�b�N��Y���͒l�B

	/* �����E�ԍ����֘A�B*/ 
	const auto ATTACK_RANGE_BASE = 150.0f;     //! ��U�������B
	const auto ATTACK_RANGE_MELEE = 150.0f;    //! �ߐڐE�̊�U�������B
	const auto ATTACK_RANGE_MAGIC = 250.0f;    //! �������E�̊�U�������B

	const auto ATTACK_RANGE_SWORD = 20.0f;     //! ���̍U�������B
	const auto ATTACK_RANGE_HAMMER = 40.0f;    //! �n���}�[�̍U�������B
	const auto ATTACK_RANGE_WAND = 80.0f;      //! ��̍U�������B
	const auto ATTACK_RANGE_GUN = 130.0f;      //! �e�̍U�������B
	
	const auto HELP_RANGE = 90.0f;             //! �������~���\�ȋ����B
	const auto MOVE_DEAD_ZONE = 0.001f;        //! ���K���O�̈��S����B
	const auto HOLD_FRAME = 3;				   //! �{�^��������������t���[�����i�U���̎������Ԃɉe���j�B
}

namespace nsApp
{
	/* ���u���C���c NPCBrain�N���X���w���܂��B�@NPC�̔��f(�])���i���Ă��܂��B*/
	namespace nsState
	{
		void NPCChaseState::Enter()
		{
			/* �u���C���ƃR���|�[�l���g�̎擾�B*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			/* �u���C�����Ȃ��ꍇ�͂��̏�Ԃ��ێ��ł��Ȃ����߁A�����ɔ�����B*/
			if (m_brain == nullptr)
			{
				m_body = nullptr;
				m_vInput = nullptr;
				return;
			}

			/* �u���C������K�v�ȃR���|�[�l���g���擾�B*/
			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* �R���|�[�l���g���Ȃ��ꍇ�͂��̏�Ԃ��ێ��ł��Ȃ����߁A�����ɔ�����B*/
			ClearMoveInput();
		}


		void NPCChaseState::Update()
		{
			/* �u���C���ƃR���|�[�l���g���Ȃ��ꍇ�͂��̏�Ԃ��ێ��ł��Ȃ����߁A�����ɔ�����B*/
			if (!m_body || !m_vInput || !m_brain)
				return;

			/* �����̋~����D�悷��B*/
			auto* helpTarget = m_brain->GetHelpTarget();

			/* �~���Ώۂ����݂�����ꍇ�B*/
			if (helpTarget != nullptr)
			{
				/* ���A�����ȊO�ŁA�����S���Ă��邩HP��0�ȉ��̏ꍇ�͋~���s���Ɉڂ�B*/
				if (helpTarget != m_body && (helpTarget->IsDeath() || helpTarget->GetCharacterStatus().hp.currentHP <= 0))
				{
					/* �~���s�������s�B*/
					ClearMoveInput();

					/* �~���s���Ɉڂ�B*/
					if (m_stateMachine != nullptr)
						m_stateMachine->ChangeState(new NPCHelpState(helpTarget));

					return;
				}
			}

			/* �~���Ώۂ����݂��Ȃ��A�������͋~���̕K�v���Ȃ��ꍇ�́A�U���Ώۂ�ǂ�������B*/
			auto* target = m_brain->SearchTarget();
			ExecuteChaseAction(target);
		}


		void NPCChaseState::Exit()
		{
			/* �u���C���ƃR���|�[�l���g���Ȃ��ꍇ�͂��̏�Ԃ��ێ��ł��Ȃ����߁A�����ɔ�����B*/
			ClearMoveInput();

			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
		}


		bool NPCChaseState::ExecuteHelpAction(nsActor::Player* helpTarget)
		{
			/* �~���ΏۂƃR���|�[�l���g���Ȃ��ꍇ�͋~���s�������s�ł��Ȃ����߁Afalse��Ԃ��B*/
			if (helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return false;

			/* �~���ΏۂƂ̋������v�Z�B*/
			ComputeDistance(helpTarget);
			
			/* �~���Ώۂ��~���\�ȋ�����艓���ꍇ�́A�~���Ώۂɋ߂Â��B*/
			if (m_distance > HELP_RANGE)
				MoveTowardTarget();

			else
			{
				/* �~���Ώۂ��~���\�ȋ����ɂ���ꍇ�́A�~���s�������s����B*/
				ClearMoveInput();
				m_vInput->RequestButton(enButtonY, HOLD_FRAME);
			}

			return true;
		}


		void NPCChaseState::ExecuteChaseAction(nsActor::ICharacter* target)
		{
			if (target == nullptr)
			{
				/* �ڕW�����Ȃ��ꍇ�͒ǐՏ�ԂɑJ�ڂ���B*/
				ClearMoveInput();

				/* �ڕW�����Ȃ��ꍇ�͑ҋ@��ԂɑJ�ڂ���B*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCIdleState());

				return;
			}

			m_myWeapon = m_body->GetCurrentWeapon();
			m_attackRange = CharacterToBeChosen(m_myWeapon);

			ComputeDistance(target);

			if (m_distance > m_attackRange)
			{
				MoveTowardTarget();
				return;
			}

			/* �U���͈͓��ł͕K���~�܂�B
			 * �ȑO�̉��ړ������́A�U���ҋ@���ɖ��t���[�����͂����葱���邽�߁A
			 * NPC�������ō��E�ɂ��낤�낷�錴���ɂȂ�₷���B
			 */
			ClearMoveInput();

			if (!m_brain->CanAttack())
				return;

			TransitionToAttackState();
		}
		

		void NPCChaseState::TransitionToAttackState()
		{
			/* �u���C���ƃR���|�[�l���g���Ȃ��ꍇ�͍U����ԂɑJ�ڂł��Ȃ����߁A�����ɔ�����B*/
			if (m_stateMachine == nullptr || m_body == nullptr)
				return;

			/* �U����ԂɑJ�ڂ���O�ɁA�ړ����͂����Z�b�g����NPC���U�����ɕs���R�ɓ������̂�h�~����B*/
			ClearMoveInput();

			/* ���݂̕�����擾���āA�U����ԂɑJ�ڂ���B*/
			m_myWeapon = m_body->GetCurrentWeapon();

			/* ����̎�ނɉ����āA�Ή�����U����ԂɑJ�ڂ���B*/
			if (m_myWeapon == WeaponType::Wand)
				m_stateMachine->ChangeState(new NPCWandAttackState());

			else if (m_myWeapon == WeaponType::Hammer)
				m_stateMachine->ChangeState(new NPCHammerAttackState());

			else if (m_myWeapon == WeaponType::TwinGun)
				m_stateMachine->ChangeState(new NPCTwinGunAttackState());

			else
				m_stateMachine->ChangeState(new NPCSwordAttackState());
		}


		void NPCChaseState::ClearMoveInput()
		{
			/* �R���|�[�l���g���Ȃ��ꍇ�͈ړ����͂��N���A�ł��Ȃ����߁A�����ɔ�����B*/
			if (m_vInput == nullptr)
				return;

			/* �ړ����͂��j���[�g�����Ƀ��Z�b�g����B*/
			m_vInput->SetLStick(INPUT_STICK_X, INPUT_STICK_Y);
			/* ���ړ��̓��͂����Z�b�g����B*/
			m_vInput->SetButton(enButtonLB1, false);
		}


		void NPCChaseState::MoveTowardTarget()
		{
			/* �u���C���ƃR���|�[�l���g���Ȃ��ꍇ�͈ړ����͂������ł��Ȃ����߁A�����ɔ�����B*/
			if (m_vInput == nullptr)
				return;

			/* �ڕW�Ƃ̋������v�Z�B*/
			if (m_distance <= MOVE_DEAD_ZONE || m_difference.LengthSq() <= MOVE_DEAD_ZONE)
			{
				/* �ڕW�Ƃ̋��������ɋ߂��ꍇ�́A�ړ����͂��N���A���Ē�~����B*/
				ClearMoveInput();
				return;
			}

			/* �ڕW�Ƃ̋������\���ɂ���ꍇ�́A�ڕW�̕����Ɍ������Ĉړ�����B*/
			m_difference.Normalize();
			m_vInput->SetLStick(m_difference.x, m_difference.z);
			m_vInput->SetButton(enButtonLB1, false);
		}


		float NPCChaseState::CharacterToBeChosen(WeaponType type) const
		{
			/* ����̎�ނɉ����āA�U���͈͂𒲐�����B*/
			switch (type)
			{
			case WeaponType::GreatSword:
				return ATTACK_RANGE_BASE - ATTACK_RANGE_SWORD;

			case WeaponType::Hammer:
				return ATTACK_RANGE_BASE - ATTACK_RANGE_HAMMER;

			case WeaponType::Wand:
				return ATTACK_RANGE_BASE + ATTACK_RANGE_WAND;

			case WeaponType::TwinGun:
				return ATTACK_RANGE_BASE + ATTACK_RANGE_GUN;

			default:
				return ATTACK_RANGE_BASE;
			}
		}
	}
}
