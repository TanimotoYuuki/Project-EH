#include "stdafx.h"
#include "NPCWandAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCAttackBaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/Movement/NPCCombatRangeHelper.h"
#include "Src/Actor/Character/NPC/Movement/NPCMovementController.h"

namespace
{
	/* 距離・間合い関連 */
	const auto CHASE_TRANSITION_DISTANCE = 300.0f; //! 敵がこの距離より離れたら追跡ステートへ戻る。
	const auto RETREAT_DISTANCE = 100.0f;          //! 攻撃後のクールタイム中に、この距離より近ければ後ろに下がる。
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f;    //! めり込みを防止する限界距離。

	/* 時間（フレーム）関連 */
	const auto ATTACK_DURATION = 40;               //! 攻撃モーションが継続し、敵の方を向き続ける時間。
	const auto ATTACK_RESET_TIME = 70;             //! 通常の終了判定時間。
	const auto ATTACK_FORCE_RESET_TIME = 120;      //! アニメーションが終わらない場合の保険。

	/* コンボ入力のタイミング（フレーム） */
	const auto COMBO_FIRST_INPUT = 1;              //! 1段目のボタン入力タイミング
	const auto COMBO_AIR_INPUT = 15;               //! 空中攻撃（Bボタン）の入力タイミング

	/* モデルの回転角度 */
	const auto FACING_ANGLE_RIGHT = 90.0f;         //! 右を向く時の角度。
	const auto FACING_ANGLE_LEFT = -90.0f;         //! 左を向く時の角度。

	const auto HOLD_FRAME = 3;					   //! ボタンを押し続けるフレーム数（攻撃の持続時間に影響）。
	const auto STICK_NEUTRAL = 0.0f;			   //! スティックをニュートラル値。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCWandAttackState::Enter()
		{
			/* ブレインとコンポーネントの取得。*/
			NPCAttackBaseState::Enter();

			/* タイマーと状態の初期化。*/
			m_healAttempted = false;

			/* 攻撃パターンは距離計算後の Update で選択する */
		}


		void NPCWandAttackState::Update()
		{
			/* ヘルプ遷移の判定。*/
			if (CheckHelpTransition())
				return;

			/* 危険回避が必要かチェック。*/
			if (CheckEvadeTransition())
				return;

			if (m_getBody != nullptr && m_getBody->IsCastingHeal())
			{
				if (m_virtualInput != nullptr)
					m_virtualInput->Reset();
				return;
			}

			/* 回復魔法終了後の処理。*/
			if (m_currentPattern == NPCWandPattern::enMagicHeal && m_healAttempted
				&& m_getBody != nullptr && !m_getBody->IsCastingHeal())
			{
				if (m_virtualInput != nullptr)
					m_virtualInput->Reset();
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* 攻撃サイクル中でも回復需要が出たら Heal へ切り替え。*/
			if (m_npcBrain != nullptr
				&& (m_npcBrain->ShouldHealSelf() || m_npcBrain->ShouldHealAlly()))
			{
				if (m_currentPattern != NPCWandPattern::enMagicHeal || m_healAttempted)
				{
					m_currentPattern = NPCWandPattern::enMagicHeal;
					m_healAttempted = false;
					m_attackTimer = 0;
				}
			}

			nsActor::ICharacter* target = m_npcBrain->SearchTarget();
			if (target == nullptr || m_getBody == nullptr)
				return;

			ComputeDistance(target);
			const NPCBehaviorProfile& profile = m_npcBrain->GetBehaviorProfile();

			/* 近すぎたらその場で後退（Chase には戻さない）。*/
			if (nsNPC::ShouldRetreatWithHysteresis(m_distance, profile, m_npcBrain->GetIsRetreatingFlag()))
			{
				UpdateFacingDirection();
				UpdateMovement();
				m_attackTimer++;
				return;
			}

			/* 一定の距離以上離れた場合、追跡状態に遷移。*/
			const float chaseReturn = nsNPC::GetRangedChaseEnterDistance(profile) - 5.0f;
			if (m_distance > chaseReturn)
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* 攻撃サイクル開始時にパターンを選択。*/
			if (m_attackTimer == 0)
				ChoosePattern();

			m_attackTimer++;
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			ExecutionFlow();
			PreventClipping(target);

			if (m_attackTimer > ATTACK_RESET_TIME && !m_getBody->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			if (m_attackTimer > ATTACK_FORCE_RESET_TIME)
			{
				if (m_virtualInput != nullptr)
					m_virtualInput->Reset();
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}
		}


		void NPCWandAttackState::ExecuteCurrentCombo()
		{
			/* コンボがない場合。*/
			if (m_currentPattern == NPCWandPattern::enNone)
				return;

			/* 関数ポインタによるディスパッチ。*/
			void (NPCWandAttackState:: * actions[])() = {
				&NPCWandAttackState::ExecuteMagicAttack,
				&NPCWandAttackState::ExecuteMagicHeal,
				&NPCWandAttackState::ExecuteMeleeAir
			};
			(this->*actions[static_cast<int>(m_currentPattern)])();
		}


		void NPCWandAttackState::ExecuteMagicAttack()
		{
			/* 攻撃の最初のフレームでRB1を押す。*/
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonRB1, HOLD_FRAME);
		}


		void NPCWandAttackState::ExecuteMagicHeal()
		{
			if (m_attackTimer != COMBO_FIRST_INPUT)
				return;
			if (m_getBody == nullptr)
				return;

			if (m_getBody->TryBeginHeelMagic())
				m_healAttempted = true;
			else if (m_stateMachine != nullptr)
				m_stateMachine->ChangeState(new NPCChaseState());
		}


		void NPCWandAttackState::ExecuteMeleeAir()
		{
			/* 攻撃の最初のフレームでAを押す。*/
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonA, HOLD_FRAME);

			/* 空中攻撃のタイミングでBを押す。*/
			if (m_attackTimer == COMBO_AIR_INPUT)
				m_virtualInput->RequestButton(enButtonB, HOLD_FRAME);
		}


		void NPCWandAttackState::UpdateMovement()
		{
			if (m_virtualInput == nullptr || m_npcBrain == nullptr || m_getBody == nullptr)
				return;

			const NPCBehaviorProfile& profile = m_npcBrain->GetBehaviorProfile();

			if (nsNPC::ShouldRetreatWithHysteresis(m_distance, profile, m_npcBrain->GetIsRetreatingFlag()))
			{
				nsActor::ICharacter* target = m_npcBrain->SearchTarget();
				if (target != nullptr)
				{
					const float signX = nsNPC::CalcRetreatStickX(
						m_getBody->GetPosition(), target->GetPosition());
					if (fabsf(signX) > 0.001f)
					{
						Vector3 away(signX, 0.0f, 0.0f);
						NPCMovementController::Apply(
							m_virtualInput,
							NPCMovementController::MakeMoveIntent(away, true));
						return;
					}
				}
			}

			NPCMovementController::Stop(m_virtualInput);
		}


		void NPCWandAttackState::ExecutionFlow()
		{
			/* 入力のリセット。*/
			ResetVirtualInputs();

			/* 技ボタンの入力。*/
			ExecuteCurrentCombo();

			/* 向きの更新。*/
			UpdateFacingDirection();

			/* 移動/距離の計算。*/
			UpdateMovement();
		}


		void NPCWandAttackState::ChoosePattern()
		{
			/* 回復が必要なときだけ Heal。*/
			if (m_npcBrain->ShouldHealSelf() || m_npcBrain->ShouldHealAlly())
			{
				m_currentPattern = NPCWandPattern::enMagicHeal;
				return;
			}

			/* 回復不要なら攻撃（サポート職でもボス戦では攻撃する）。*/
			const NPCBehaviorProfile& profile = m_npcBrain->GetBehaviorProfile();
			const float retreatDist = nsNPC::GetRetreatDistanceByProfile(profile);

			NPCWandPattern candidates[2];
			int count = 0;
			candidates[count++] = NPCWandPattern::enMagicAttack;

			const auto AIR_MIN_DISTANCE = 60.0f;
			if (m_distance >= retreatDist + AIR_MIN_DISTANCE)
				candidates[count++] = NPCWandPattern::enAir;

			m_currentPattern = candidates[rand() % count];
		}	
	}
}