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
#include "Src/Actor/Character/NPC/Component/NPCActionParameterTable.h"

namespace
{
	/* 入力値。*/
	const auto INPUT_STICK_X = 0.0f;           //! スティックのX入力値。
	const auto INPUT_STICK_Y = 0.0f;           //! スティックのY入力値。

	/* 距離・間合い関連。*/ 
	const auto ATTACK_RANGE_BASE = 150.0f;     //! 基準攻撃距離。
	const auto ATTACK_RANGE_MELEE = 150.0f;    //! 近接職の基準攻撃距離。
	const auto ATTACK_RANGE_MAGIC = 250.0f;    //! 遠距離職の基準攻撃距離。

	const auto ATTACK_RANGE_SWORD = 20.0f;     //! 剣の攻撃距離。
	const auto ATTACK_RANGE_HAMMER = 40.0f;    //! ハンマーの攻撃距離。
	const auto ATTACK_RANGE_WAND = 80.0f;      //! 杖の攻撃距離。
	const auto ATTACK_RANGE_GUN = 130.0f;      //! 銃の攻撃距離。
	
	const auto HELP_RANGE = 90.0f;             //! 味方を救助可能な距離。
	const auto MOVE_DEAD_ZONE = 0.001f;        //! 正規化前の安全判定。
	const auto HOLD_FRAME = 3;				   //! ボタンを押し続けるフレーム数（攻撃の持続時間に影響）。
}

namespace nsApp
{
	/* ※ブレイン… NPCBrainクラスを指します。　NPCの判断(脳)を司っています。*/
	namespace nsState
	{
		void NPCChaseState::Enter()
		{
			/* ブレインとコンポーネントの取得。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			/* ブレインがない場合はこの状態を維持できないため、即座に抜ける。*/
			if (m_brain == nullptr)
			{
				m_body = nullptr;
				m_vInput = nullptr;
				return;
			}

			/* ブレインから必要なコンポーネントを取得。*/
			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* コンポーネントがない場合はこの状態を維持できないため、即座に抜ける。*/
			ClearMoveInput();
		}


		void NPCChaseState::Update()
		{
			/* ブレインとコンポーネントがない場合はこの状態を維持できないため、即座に抜ける。*/
			if (!m_body || !m_vInput || !m_brain)
				return;

			/* 味方の救助を優先する。*/
			auto* helpTarget = m_brain->GetHelpTarget();

			/* 救助対象が存在しする場合。*/
			if (helpTarget != nullptr)
			{
				/* かつ、自分以外で、かつ死亡しているかHPが0以下の場合は救助行動に移る。*/
				if (helpTarget != m_body && (helpTarget->IsDeath() || helpTarget->GetCharacterStatus().hp.currentHP <= 0))
				{
					/* 救助行動を実行。*/
					ClearMoveInput();

					/* 救助行動に移る。*/
					if (m_stateMachine != nullptr)
						m_stateMachine->ChangeState(new NPCHelpState(helpTarget));

					return;
				}
			}

			/* 救助対象が存在しない、もしくは救助の必要がない場合は、攻撃対象を追いかける。*/
			auto* target = m_brain->SearchTarget();
			ExecuteChaseAction(target);
		}


		void NPCChaseState::Exit()
		{
			/* ブレインとコンポーネントがない場合はこの状態を維持できないため、即座に抜ける。*/
			ClearMoveInput();

			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
		}


		bool NPCChaseState::ExecuteHelpAction(nsActor::Player* helpTarget)
		{
			/* 救助対象とコンポーネントがない場合は救助行動を実行できないため、falseを返す。*/
			if (helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return false;

			/* 救助対象との距離を計算。*/
			ComputeDistance(helpTarget);
			
			/* 救助対象が救助可能な距離より遠い場合は、救助対象に近づく。*/
			if (m_distance > HELP_RANGE)
				MoveTowardTarget();

			else
			{
				/* 救助対象が救助可能な距離にいる場合は、救助行動を実行する。*/
				ClearMoveInput();
				m_vInput->RequestButton(enButtonY, HOLD_FRAME);
			}

			return true;
		}


		void NPCChaseState::ExecuteChaseAction(nsActor::ICharacter* target)
		{
			if (target == nullptr)
			{
				/* 目標がいない場合は追跡状態に遷移する。*/
				ClearMoveInput();

				/* 目標がいない場合は待機状態に遷移する。*/
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
			/* ブレインとコンポーネントがない場合は攻撃状態に遷移できないため、即座に抜ける。*/
			if (m_stateMachine == nullptr || m_body == nullptr)
				return;

			/* 攻撃状態に遷移する前に、移動入力をリセットしてNPCが攻撃中に不自然に動き回るのを防止する。*/
			ClearMoveInput();

			/* 現在の武器を取得して、攻撃状態に遷移する。*/
			m_myWeapon = m_body->GetCurrentWeapon();

			/* 武器の種類に応じて、対応する攻撃状態に遷移する。*/
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
			/* コンポーネントがない場合は移動入力をクリアできないため、即座に抜ける。*/
			if (m_vInput == nullptr)
				return;

			/* 移動入力をニュートラルにリセットする。*/
			m_vInput->SetLStick(INPUT_STICK_X, INPUT_STICK_Y);
			/* 横移動の入力もリセットする。*/
			m_vInput->SetButton(enButtonLB1, false);
		}


		void NPCChaseState::MoveTowardTarget()
		{
			/* ブレインとコンポーネントがない場合は移動入力を処理できないため、即座に抜ける。*/
			if (m_vInput == nullptr)
				return;

			/* 目標との距離を計算。*/
			if (m_distance <= MOVE_DEAD_ZONE || m_difference.LengthSq() <= MOVE_DEAD_ZONE)
			{
				/* 目標との距離が非常に近い場合は、移動入力をクリアして停止する。*/
				ClearMoveInput();
				return;
			}

			/* 目標との距離が十分にある場合は、目標の方向に向かって移動する。*/
			m_difference.Normalize();
			m_vInput->SetLStick(m_difference.x, m_difference.z);
			m_vInput->SetButton(enButtonLB1, false);
		}


		float NPCChaseState::CharacterToBeChosen(WeaponType type) const
		{
			/* 武器の種類に応じて、攻撃範囲を調整する。*/
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
