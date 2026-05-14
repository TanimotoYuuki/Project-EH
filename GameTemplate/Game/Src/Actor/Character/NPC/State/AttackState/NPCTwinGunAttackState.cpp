#include "stdafx.h"
#include "NPCTwinGunAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace
{
	/* 乱数・パターン関連 */
	const auto NUM_TWINGUN_PATTERNS = 3;           //! 双銃NPCが持つ攻撃パターンの総数

	/* 距離・間合い関連 */
	const auto CHASE_TRANSITION_DISTANCE = 150.0f; //! 敵がこの距離より離れたら追跡ステートへ戻る
	const auto RETREAT_DISTANCE = 80.0f;           //! 攻撃後のクールタイム中に、近ければ後ろに下がる
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f;    //! めり込みを防止する限界距離

	/* 時間（フレーム）関連 */
	const auto ATTACK_DURATION = 40;               //! 攻撃モーションが継続し、敵の方を向き続ける時間
	const auto ATTACK_RESET_TIME = 70;             //! 次の行動（再抽選）に移るまでの総時間

	/* コンボ入力のタイミング（フレーム） */
	const auto COMBO_FIRST_INPUT = 1;              //! 1段目のボタン入力タイミング
	const auto COMBO_SECOND_INPUT = 10;            //! B連打用
	const auto COMBO_THIRD_INPUT = 20;             //! B連打用

	const auto COMBO_AIR_INPUT = 15;               //! 空中攻撃（Bボタン）の入力タイミング
}

namespace nsApp
{
	namespace nsState
	{
		void NPCTwinGunAttackState::Enter()
		{
			NPCAttackBaseState::Enter();

			m_randomPattern = rand() % NUM_TWINGUN_PATTERNS;
			m_currentPattern =
				(m_randomPattern == 0) ? NPCTwinGunPattern::enRush :
				(m_randomPattern == 1) ? NPCTwinGunPattern::enHeavy :
				NPCTwinGunPattern::enAir;
		}

		void NPCTwinGunAttackState::Update()
		{
			auto target = m_npcBrain->SearchTarget();

			if (!target || !m_getBody)
				return;

			ComputeDistance(target);

			if (m_distance > CHASE_TRANSITION_DISTANCE) {
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			m_attackTimer++;

			m_diff.Normalize();
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			ExecutionFlow();

			PreventClipping(target);

			if (m_attackTimer > ATTACK_RESET_TIME)
				Enter();
		}

		void NPCTwinGunAttackState::ExecuteCurrentCombo()
		{
			if (m_currentPattern == NPCTwinGunPattern::enNone)
				return;

			void (NPCTwinGunAttackState:: * actions[])() = {
				&NPCTwinGunAttackState::ExecuteShootRush,
				&NPCTwinGunAttackState::ExecuteShootHeavy,
				&NPCTwinGunAttackState::ExecuteShootAir
			};
			(this->*actions[static_cast<int>(m_currentPattern)])();
		}

		void NPCTwinGunAttackState::ExecuteShootRush()
		{
			/* Bボタンを複数回押して乱射（RushAttack）を誘発 */
			if (m_attackTimer == COMBO_FIRST_INPUT || m_attackTimer == COMBO_SECOND_INPUT || m_attackTimer == COMBO_THIRD_INPUT)
				m_virtualInput->SetButton(enButtonB,true);
		}

		void NPCTwinGunAttackState::ExecuteShootHeavy()
		{
			/* Xボタンで重攻撃（爆発弾）を撃つ */
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->SetButton(enButtonX, true);
		}

		void NPCTwinGunAttackState::ExecuteShootAir()
		{
			/* Aボタンでジャンプし、空中でBを押して射撃 */
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->SetButton(enButtonA,true);

			if (m_attackTimer == COMBO_AIR_INPUT)
				m_virtualInput->SetButton(enButtonB, true);
		}

		void NPCTwinGunAttackState::UpdateMovement()
		{
			/* 銃は遠距離なので攻撃中は立ち止まり、近すぎたら下がる */
			m_stickX = m_isAttacking ? 0.0f : (m_distance < RETREAT_DISTANCE ? -m_diff.x : 0.0f);
			m_stickZ = m_isAttacking ? 0.0f : (m_distance < RETREAT_DISTANCE ? -m_diff.z : 0.0f);

			m_virtualInput->SetLStick(m_stickX, m_stickZ);
		}

		void NPCTwinGunAttackState::ExecutionFlow()
		{
			ResetVirtualInputs();
			ExecuteCurrentCombo();
			UpdateFacingDirection();
			UpdateMovement();
		}
	}
}