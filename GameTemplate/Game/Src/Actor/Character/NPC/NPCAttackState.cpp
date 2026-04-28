#include "stdafx.h"
#include "NPCAttackState.h"
#include "Src/Actor/Character/NPC/NPCChaseState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		void NPCAttackState::Enter()
		{
			m_brain = static_cast<NPCBrain*>(m_owner);
			m_attackTimer = 0.0f;

			/* */
			auto body = m_brain->GetBody();
			if (!body)
				return;

			/* 武器の情報を取得する。*/
			m_myWeaponType = body->GetCurrentWeapon();

			/* 武器によって派生。*/
			if (m_myWeaponType == WeaponType::Wand)
			{
				/* HPの割合が半分以下なら回復魔法を最優先として発動。*/
			}
		}


		void NPCAttackState::Update()
		{
			auto target = m_brain->SearchTarget();
			auto body = m_brain->GetBody();

			if (target == nullptr || body == nullptr)
				return;

			/* 敵との距離を測る */
			Vector3 diff = target->GetPosition() - body->GetPosition();
			float distance = diff.Length();

			/* 敵が離れたら（または自分が吹き飛ばされたら）、再び追いかける */
			/* ※Chaseのしきい値(80.0f)より少し広めにしておくと挙動が安定します */
			if (distance > 100.0f)
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			m_attackTimer++;

			/* 約1秒（60フレーム）のテンポで、仮想のBボタンを押して離す！ */
			if (m_attackTimer % 60 == 0)
			{
				/* 仮想Bボタンを「押す」 */
				body->GetInputClass().SetVirtualButtonB(true);
			}
			else if (m_attackTimer % 60 == 5)
			{
				/* 5フレーム後に仮想Bボタンを「離す」 */
				/* （これで PlayerInput 側で「トリガー（攻撃）」と「短押し（通常攻撃）」の条件が成立します！） */
				body->GetInputClass().SetVirtualButtonB(false);
			}
		}

		void NPCAttackState::Exit()
		{
			/* 念のため、ステートを抜ける時はBボタンを離しておく */
			auto body = m_brain->GetBody();
			if (body) {
				body->GetInputClass().SetVirtualButtonB(false);
			}
		}
	}
}