#include "stdafx.h"
#include "NPCAttackState.h"
#include "Src/Actor/Character/NPC/NPCChaseState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/PlayerInput.h"

namespace nsApp
{
	namespace nsState
	{
		void NPCAttackState::Enter()
		{
			m_brain = static_cast<NPCBrain*>(m_owner);
			m_attackTimer = 0;

			auto body = m_brain->GetBody();
			if (!body) return;

			m_myWeaponType = body->GetCurrentWeapon();

			/* 🧠 各キャラクターの「個性（持っている技）」に合わせて抽選！ */
			if (m_myWeaponType == WeaponType::Wand)
			{
				if (body->GetCharacterStatus().hp.currentHP <= (body->GetCharacterStatus().hp.maxHP / 2))
					m_currentPattern = NPCComboPattern::enMagic_Heal;
				else
					m_currentPattern = (rand() % 2 == 0) ? NPCComboPattern::enMagic_Attack : NPCComboPattern::enMelee_Air;
			}
			else if (m_myWeaponType == WeaponType::Hammer)
			{
				int r = rand() % 3;
				if (r == 0)      m_currentPattern = NPCComboPattern::enMelee_Heavy;
				else if (r == 1) m_currentPattern = NPCComboPattern::enMelee_Push;
				else             m_currentPattern = NPCComboPattern::enMelee_Air;
			}
			else
			{
				int r = rand() % 3;
				if (r == 0)      m_currentPattern = NPCComboPattern::enMelee_Air;
				else if (r == 1) m_currentPattern = NPCComboPattern::enMelee_Push;
				else             m_currentPattern = NPCComboPattern::enMelee_Rush;
			}
		}

		void NPCAttackState::Update()
		{
			auto target = m_brain->SearchTarget();
			auto body = m_brain->GetBody();

			if (target == nullptr || body == nullptr) return;

			Vector3 diff = target->GetPosition() - body->GetPosition();
			float distance = diff.Length();

			if (distance > 100.0f)
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			m_attackTimer++;
			auto& inputsys = body->GetInputClass();

			/* 毎フレーム入力をリセット */
			inputsys.SetVirtualButtonA(false);
			inputsys.SetVirtualButtonB(false);
			inputsys.SetVirtualButtonX(false);
			inputsys.SetVirtualButtonLB1(false);
			inputsys.SetVirtualButtonLB2(false);
			inputsys.SetVirtualButtonRB1(false);
			inputsys.SetVirtualButtonRT(false);

			switch (m_currentPattern)
			{
			case NPCComboPattern::enMelee_Rush:   ExecuteMeleeRush(body);   break;
			case NPCComboPattern::enMelee_Heavy:  ExecuteMeleeHeavy(body);  break;
			case NPCComboPattern::enMelee_Push:   ExecuteMeleePush(body);   break;
			case NPCComboPattern::enMelee_Air:    ExecuteMeleeAir(body);    break;
			case NPCComboPattern::enMagic_Attack: ExecuteMagicAttack(body); break;
			case NPCComboPattern::enMagic_Heal:   ExecuteMagicHeal(body);   break;
			default: break;
			}

			/* スティック入力の制御 */
			diff.Normalize();
			if (m_attackTimer < 40)
			{
				if (diff.x > 0.0f) {
					body->SetAngle(90.0f);
					body->SetForwardVector(Vector3::Right);
				}
				else {
					body->SetAngle(-90.0f);
					body->SetForwardVector(Vector3::Left);
				}

				if (m_currentPattern == NPCComboPattern::enMelee_Push ||
					m_currentPattern == NPCComboPattern::enMelee_Rush)
				{
					inputsys.SetVirtualController(diff.x, diff.z);
				}
				else
				{
					inputsys.SetVirtualController(0.0f, 0.0f);
				}
			}
			else
			{
				float appropriateDistance = (m_myWeaponType == WeaponType::Wand) ? 100.0f : 50.0f;
				if (distance < appropriateDistance)
					inputsys.SetVirtualController(-diff.x, -diff.z);
				else
					inputsys.SetVirtualController(0.0f, 0.0f);
			}

			/* ================================================== */
			/* 🛡️ 強制めり込み防止処理（疑似コリジョンバリア） */
			/* ================================================== */
			float limitDistance = 40.0f; /* これ以上は近づかない限界距離 */
			if (distance < limitDistance && distance > 0.0f)
			{
				/* 敵から自分へ向かうベクトル（押し戻す方向）を作る */
				Vector3 pushBackDir = body->GetPosition() - target->GetPosition();
				pushBackDir.y = 0.0f; // 高さは変えない
				pushBackDir.Normalize();

				/* めり込んだ分だけ座標を強制的に外側へ押し戻す！ */
				Vector3 correctedPos = body->GetPosition();
				correctedPos.x += pushBackDir.x * (limitDistance - distance);
				correctedPos.z += pushBackDir.z * (limitDistance - distance);

				body->GetCharacterController().SetPosition(correctedPos);
				body->SetPosition(correctedPos);
			}

			if (m_attackTimer > 70)
			{
				Enter();
			}
		}

		void NPCAttackState::Exit()
		{
			auto body = m_brain->GetBody();
			if (body) {
				body->GetInputClass().SetVirtualButtonB(false);
				body->GetInputClass().SetVirtualController(0.0f, 0.0f);
			}
		}

		/* ⚔️ コンボ入力関数の実装 */
		void NPCAttackState::ExecuteMeleeAir(nsActor::Player* body)
		{
			auto& inputsys = body->GetInputClass();
			if (m_attackTimer == 1)  inputsys.SetVirtualButtonA(true);
			if (m_attackTimer == 15) inputsys.SetVirtualButtonB(true);
		}

		void NPCAttackState::ExecuteMeleeHeavy(nsActor::Player* body)
		{
			auto& inputsys = body->GetInputClass();
			if (m_attackTimer == 1)  inputsys.SetVirtualButtonB(true);
			if (m_attackTimer == 15) inputsys.SetVirtualButtonX(true);
		}

		void NPCAttackState::ExecuteMeleePush(nsActor::Player* body)
		{
			auto& inputsys = body->GetInputClass();
			if (m_attackTimer < 10)  inputsys.SetVirtualButtonLB1(true);
			if (m_attackTimer == 5)  inputsys.SetVirtualButtonB(true);
		}

		void NPCAttackState::ExecuteMeleeRush(nsActor::Player* body)
		{
			auto& inputsys = body->GetInputClass();
			if (m_attackTimer == 1)  inputsys.SetVirtualButtonB(true);
			if (m_attackTimer == 15) inputsys.SetVirtualButtonB(true);
			if (m_attackTimer == 30) inputsys.SetVirtualButtonB(true);
		}

		void NPCAttackState::ExecuteMagicAttack(nsActor::Player* body)
		{
			auto& inputsys = body->GetInputClass();
			if (m_attackTimer == 1)  inputsys.SetVirtualButtonRB1(true);
		}

		void NPCAttackState::ExecuteMagicHeal(nsActor::Player* body)
		{
			auto& inputsys = body->GetInputClass();
			if (m_attackTimer == 1)  inputsys.SetVirtualButtonRT(true);
		}
	}
}