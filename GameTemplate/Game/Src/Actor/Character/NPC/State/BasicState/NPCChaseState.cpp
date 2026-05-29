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
	const auto HELP_RANGE = 80.0f;           //! 味方を救助可能な距離
	const auto ATTACK_RANGE_MELEE = 150.0f;   //! 近接職（剣・ハンマー）が攻撃を開始する距離
	const auto ATTACK_RANGE_MAGIC = 250.0f;  //! 遠距離職（杖）が攻撃を開始する距離
}

namespace nsApp
{
	namespace nsState
	{
		void NPCChaseState::Enter()
		{
			/* キャスト。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			m_body = m_brain->GetBody();
			if(m_body)
				m_vInput = m_brain->GetVirtualInputAdapter();
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
					m_vInput->Reset();

					if (m_stateMachine != nullptr)
						m_stateMachine->ChangeState(new NPCHelpState(helpTarget));

					return;
				}
			}

			auto target = m_brain->SearchTarget();
			ExecuteChaseAction(target);
		}

		bool NPCChaseState::ExecuteHelpAction(nsActor::Player* helpTarget)
		{
			if (helpTarget == nullptr)
				return false;

			ComputeDistance(helpTarget);

			m_difference = helpTarget->GetPosition() - m_body->GetPosition();
			if (m_difference.Length() > 80.0f)
			{
				/* 距離が遠ければ近づく。*/
				m_difference.Normalize();
				m_vInput->SetLStick(m_difference.x, m_difference.z);
			}

			else
			{
				/* 近づくとYボタンで救助。*/
				m_vInput->SetLStick(0.0f, 0.0f);
				m_vInput->RequestButton(enButtonY,3);
			}
			return true;
		}


		void NPCChaseState::ExecuteChaseAction(nsActor::ICharacter* target)
		{
			if (target == nullptr)
			{
				/* 敵がいないなら待機ステートへ */
				m_stateMachine->ChangeState(new NPCIdleState());
				return;
			}

			/* 武器に応じて攻撃開始距離を変える */
			m_myWeapon = m_body->GetCurrentWeapon();
			m_attackRange = CharacterToBeChosen(m_myWeapon);

			/* 距離を計算する。*/
			ComputeDistance(target);

			/* 距離を検知。*/
			if (m_distance > m_attackRange)
			{
				/* 距離が遠ければ近づく */
				m_difference.Normalize();
				m_vInput->SetLStick(m_difference.x, m_difference.z);
			}
			else
			{
				/* 近づいたら立ち止まって攻撃ステートへ遷移 */
				m_vInput->SetLStick(0.0f, 0.0f);

				/* 攻撃ができるかチェックする。*/
				if (!m_brain->CanAttack())
				{
					m_sideMove = Vector3(-m_difference.x, 0.0f, m_difference.z);
					if (m_sideMove.LengthSq() > 0.001f)
					{
						/* 正規化。*/
						m_sideMove.Normalize();
						m_vInput->SetLStick(m_sideMove.x * 0.4f, m_sideMove.z * 0.4f);
					}
					return;
				}

				/* 攻撃ステートへ遷移する。*/
				TransitionToAttackState();
			}
		}


		void NPCChaseState::TransitionToAttackState()
		{
			/* 現在の武器を取得する。*/
			m_myWeapon = m_body->GetCurrentWeapon();

			/* 杖の場合。*/
			if (m_myWeapon == WeaponType::Wand)
				m_stateMachine->ChangeState(new NPCWandAttackState());

			/* ハンマーの場合。*/
			else if (m_myWeapon == WeaponType::Hammer)
				m_stateMachine->ChangeState(new NPCHammerAttackState());

			/* それ以外の場合。*/
			else if (m_myWeapon == WeaponType::TwinGun)
				m_stateMachine->ChangeState(new NPCTwinGunAttackState());

			else
				m_stateMachine->ChangeState(new NPCSwordAttackState());
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