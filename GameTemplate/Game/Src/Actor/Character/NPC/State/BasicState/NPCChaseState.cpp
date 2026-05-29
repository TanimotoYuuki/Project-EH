#include "stdafx.h"
#include "NPCChaseState.h"
#include "Boss.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Actor/Character/NPC/State/AttackState/NPCSwordAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCHammerAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCWandAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCTwinGunAttackState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
#include "Src/Actor/Character/NPC/Component/NPCCombatHelper.h"

namespace
{
	const auto HELP_RANGE = 80.0f;             //! 味方を救助可能な距離。
	const auto ATTACK_RANGE_MELEE = 150.0f;    //! 近接職の基準攻撃距離。
	const auto ATTACK_RANGE_MAGIC = 250.0f;    //! 遠距離職の基準攻撃距離。
	const auto MOVE_DEAD_ZONE = 0.001f;        //! 正規化前の安全判定。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCChaseState::Enter()
		{
			m_brain = static_cast<NPCBrain*>(m_owner);
			if (m_brain == nullptr)
			{
				m_body = nullptr;
				m_vInput = nullptr;
				return;
			}

			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			ClearMoveInput();
		}

		void NPCChaseState::Update()
		{
			if (!m_body || !m_vInput || !m_brain)
				return;

			auto* helpTarget = m_brain->GetHelpTarget();

			if (helpTarget != nullptr)
			{
				if (helpTarget != m_body &&
					(helpTarget->IsDeath() || helpTarget->GetCharacterStatus().hp.currentHP <= 0))
				{
					ClearMoveInput();

					if (m_stateMachine != nullptr)
						m_stateMachine->ChangeState(new NPCHelpState(helpTarget));

					return;
				}
			}

			auto* target = m_brain->SearchTarget();
			ExecuteChaseAction(target);
		}

		void NPCChaseState::Exit()
		{
			ClearMoveInput();

			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
		}

		bool NPCChaseState::ExecuteHelpAction(nsActor::Player* helpTarget)
		{
			if (helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return false;

			ComputeDistance(helpTarget);

			if (m_distance > HELP_RANGE)
			{
				MoveTowardTarget();
			}
			else
			{
				ClearMoveInput();
				m_vInput->RequestButton(enButtonY, 3);
			}

			return true;
		}

		void NPCChaseState::ExecuteChaseAction(nsActor::ICharacter* target)
		{
			if (target == nullptr)
			{
				ClearMoveInput();

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

			/* 攻撃範囲内では必ず止まる。
			 * 以前の横移動処理は、攻撃待機中に毎フレーム入力が入り続けるため、
			 * NPCが高速で左右にうろうろする原因になりやすい。
			 */
			ClearMoveInput();

			if (!m_brain->CanAttack())
				return;

			TransitionToAttackState();
		}

		void NPCChaseState::TransitionToAttackState()
		{
			if (m_stateMachine == nullptr || m_body == nullptr)
				return;

			ClearMoveInput();

			m_myWeapon = m_body->GetCurrentWeapon();

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
			if (m_vInput == nullptr)
				return;

			m_vInput->SetLStick(0.0f, 0.0f);
			m_vInput->SetButton(enButtonLB1, false);
		}

		void NPCChaseState::MoveTowardTarget()
		{
			if (m_vInput == nullptr)
				return;

			if (m_distance <= MOVE_DEAD_ZONE || m_difference.LengthSq() <= MOVE_DEAD_ZONE)
			{
				ClearMoveInput();
				return;
			}

			m_difference.Normalize();
			m_vInput->SetLStick(m_difference.x, m_difference.z);
			m_vInput->SetButton(enButtonLB1, false);
		}

		float NPCChaseState::CharacterToBeChosen(WeaponType type) const
		{
			switch (type)
			{
			case WeaponType::GreatSword:
				return 130.0f;

			case WeaponType::Hammer:
				return 110.0f;

			case WeaponType::Wand:
				return 230.0f;

			case WeaponType::TwinGun:
				return 280.0f;

			default:
				return 150.0f;
			}
		}
	}
}
