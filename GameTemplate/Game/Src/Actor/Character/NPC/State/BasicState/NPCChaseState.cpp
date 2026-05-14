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
			auto target = m_brain->SearchTarget();
			auto helpTarget = m_brain->GetHelpTarget();


			/* Playerクラスを参照できているかチェック。*/
			if (!m_body || !m_vInput)
				return;

			/* 救出対象がいれば救出行動を優先する。*/
			if (ExecuteHelpAction(helpTarget))
				return;

			/* 救出対象が居なければ敵を追いかける。*/
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
				m_vInput->SetButton(enButtonY,true);
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
			m_attackRange = (m_myWeapon == WeaponType::Wand || m_myWeapon == WeaponType::TwinGun) ? ATTACK_RANGE_MAGIC : ATTACK_RANGE_MELEE;

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
	}
}