#include "stdafx.h"
#include "NPCChaseState.h"
#include "Src/Actor/Character/NPC/NPCIdleState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/NPC/NPCAttackState.h"

namespace nsApp
{
	namespace nsState
	{
		void NPCChaseState::Enter()
		{
			m_brain = static_cast<NPCBrain*>(m_owner);
		}


		void NPCChaseState::Update()
		{
			auto target = m_brain->SearchTarget();
			auto helpTarget = m_brain->GetHelpTarget();
			auto body = m_brain->GetBody();
				
			/* Playerクラスを参照できているかチェック。*/
			if (body == nullptr)
				return;

			/* 救出対象がいれば救出行動を優先する。*/
			if (ExecuteHelpAction(body, helpTarget))
				return;

			/* 救出対象が居なければ敵を追いかける。*/
			ExecuteChaseAction(body, target);
		}


		bool NPCChaseState::ExecuteHelpAction(nsActor::Player* body, nsActor::Player* helpTarget)
		{
			if (helpTarget == nullptr)
				return false;

			m_difference = helpTarget->GetPosition() - body->GetPosition();
			if (m_difference.Length() > 80.0f)
			{
				/* 距離が遠ければ近づく。*/
				m_difference.Normalize();
				body->GetInputClass().SetVirtualController(m_difference.x, m_difference.z);
			}

			else
			{
				/* 近づくとYボタンで救助。*/
				body->GetInputClass().SetVirtualController(0.0f, 0.0f);
				body->GetInputClass().SetVirtualButtonY(true);
			}
			return true;
		}


		void NPCChaseState::ExecuteChaseAction(nsActor::Player* body, nsActor::Sandbag* target)
		{
			/* */
			if (target == nullptr)
			{
				/* 待機ステートを生成。*/
				m_stateMachine->ChangeState(new NPCIdleState());
				return;
			}


			auto attackRange = 60.0f;
			if (body->GetCurrentWeapon() == WeaponType::Wand)
				attackRange = 100.0f;

			m_difference = target->GetPosition() - body->GetPosition();
			if (m_difference.Length() > attackRange)
			{
				/* 距離が遠ければ近づく。*/
				m_difference.Normalize();
				body->GetInputClass().SetVirtualController(m_difference.x, m_difference.z);
			}

			else
			{
				/* 近づいたら攻撃ステートに。*/
				body->GetInputClass().SetVirtualController(0.0f, 0.0f);
				m_stateMachine->ChangeState(new NPCAttackState());
			}
		}
	}
}