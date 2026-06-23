#include "stdafx.h"
#include "NPCSwordAttackState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/Movement/NPCCombatRangeHelper.h"

namespace
{
	/* 乱数・パターン関連 */
	const auto NUM_SWORD_PATTERNS = 3;             //! 剣NPCが持つ攻撃パターンの総数。
	const auto PATTERN_ZERO = 0;                   //! 乱数のパターン0。
	const auto PATTERN_ONE = 1;                    //! 乱数のパターン1。

	/* 距離・間合い関連 */
	const auto CHASE_TRANSITION_DISTANCE = 180.0f; //! 敵がこの距離より離れたら追跡ステートへ戻る。
	const auto RETREAT_DISTANCE = 50.0f;           //! 攻撃後のクールタイム中に、この距離より近ければ後ろに下がる。
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f;    //! めり込みを防止する限界距離（見えない壁）。

	/* 時間（フレーム）関連 */
	const auto ATTACK_DURATION = 40;               //! 攻撃モーションが継続し、敵の方を向き続ける時間。
	const auto ATTACK_RESET_TIME = 70;             //! 次の行動（再抽選）に移るまでの総時間。

	/* コンボ入力のタイミング（フレーム） */
	const auto COMBO_FIRST_INPUT = 1;              //! 1段目のボタン入力タイミング。
	const auto COMBO_SECOND_INPUT = 15;            //! 2段目のボタン入力タイミング。
	const auto COMBO_THIRD_INPUT = 30;             //! 3段目のボタン入力タイミング。

	const auto PUSH_START_INPUT = 5;               //! ダッシュ攻撃（Push）のBボタン入力タイミング。
	const auto PUSH_HOLD_DURATION = 10;            //! ダッシュ攻撃時のLB1（ダッシュ）ボタン長押し時間。

	const auto AIR_FIRST_INPUT = 1;               //! 空中攻撃の1段目の入力タイミング。
	const auto AIR_SECOND_INPUT = 15;             //! 空中攻撃の2段目の入力タイミング。

	/* モデルの回転角度 */
	const auto FACING_ANGLE_RIGHT = 90.0f;         //! 右を向く時の角度。
	const auto FACING_ANGLE_LEFT = -90.0f;         //! 左を向く時の角度。

	const auto HOLD_FRAME = 3;					   //! ボタンを押し続けるフレーム数。
	const auto STICK_NEUTRAL = 0.0f;			   //! スティックをニュートラル値。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCSwordAttackState::Enter()
		{
			/* 親クラスで初期化を行う。*/
			NPCAttackBaseState::Enter();
		}


		void NPCSwordAttackState::Update()
		{
			/* 救助要請がないかチェック。*/
			if (CheckHelpTransition())
				return;

			/* 危険回避が必要かチェック。*/
			if (CheckEvadeTransition())
				return;

			/* 目標を探索する。*/
			nsActor::ICharacter* target = m_npcBrain->SearchTarget();

			/* 早期リターン。*/
			if (target == nullptr || m_getBody == nullptr)
			{
				/* 追跡ステートに遷移。*/
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* 距離を計算。*/
			ComputeDistance(target);

			/* 届かない距離なら追跡へ戻る。*/
			if (m_distance > nsNPC::GetMeleeMaxAttackRange(m_getBody->GetCurrentWeapon()))
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* 攻撃パターンの選択。*/
			if(m_attackTimer == 0)
				ChoosePattern();

			/* タイマーの更新。*/
			m_attackTimer++;

			/* ベクトルの正規化。*/
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			/* 実行フロー。*/
			ExecutionFlow();

			/* めり込み防止。*/
			PreventClipping(target);

			/* 攻撃時間が終了したら、次の行動に移る。*/
			if (m_attackTimer > ATTACK_RESET_TIME)
				Enter();
		}


		void NPCSwordAttackState::ExecuteCurrentCombo()
		{
			/* コンボがない場合。*/
			if (m_currentPattern == NPCSwordPattern::enNone)
				return;

			/* 関数ポインタ。*/
			void (NPCSwordAttackState:: * actions[])() = {
				&NPCSwordAttackState::ExecuteMeleeRush,
				&NPCSwordAttackState::ExecuteMeleePush,
				&NPCSwordAttackState::ExecuteMeleeAir
			};
			(this->*actions[static_cast<int>(m_currentPattern)])();
		}


		void NPCSwordAttackState::ExecuteMeleeRush()
		{
			/* 攻撃のタイミングでBボタンを押す。*/ 
			/* 1段目の攻撃入力。*/ 
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonB, HOLD_FRAME);

			/* 2段目の攻撃入力。*/
			if (m_attackTimer == COMBO_SECOND_INPUT)
				m_virtualInput->RequestButton(enButtonB, HOLD_FRAME);

			/* 3段目の攻撃入力。*/
			if (m_attackTimer == COMBO_THIRD_INPUT)
				m_virtualInput->RequestButton(enButtonB, HOLD_FRAME);
		}


		void NPCSwordAttackState::ExecuteMeleePush()
		{
			/* ダッシュ攻撃の開始入力。*/
			/* LB1を押してダッシュ開始。*/
			/* 1段目の攻撃入力。*/
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonA, HOLD_FRAME);

			/* ダッシュの持続入力。*/
			/* LB1を長押ししてダッシュを持続。*/
			if (m_attackTimer == COMBO_SECOND_INPUT)
				m_virtualInput->RequestButton(enButtonB, HOLD_FRAME);
		}


		void NPCSwordAttackState::ExecuteMeleeAir()
		{
			/* 空中攻撃の1段目の入力。*/
			if (m_attackTimer == AIR_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonA, HOLD_FRAME);

			/* 空中攻撃の2段目の入力。*/
			if (m_attackTimer == AIR_SECOND_INPUT)
				m_virtualInput->RequestButton(enButtonB, HOLD_FRAME);
		}


		void NPCSwordAttackState::UpdateMovement()
		{
			/* 攻撃中は敵の方を向き続ける。*/
			m_stickX = m_isAttacking ? STICK_NEUTRAL : (m_distance < RETREAT_DISTANCE ? -m_diff.x : STICK_NEUTRAL);
			m_stickZ = m_isAttacking ? STICK_NEUTRAL : (m_distance < RETREAT_DISTANCE ? -m_diff.z : STICK_NEUTRAL);

			/* スティック入力を仮想入力に反映。*/
			m_virtualInput->SetLStick(m_stickX, m_stickZ);
		}


		void NPCSwordAttackState::ExecutionFlow()
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


		void NPCSwordAttackState::ChoosePattern()
		{
			NPCSwordPattern candidates[3];
			int count = 0;

			/* Rush：近距離のみ */
			if (m_distance <= 70.0f)
				candidates[count++] = NPCSwordPattern::enRush;

			/* Push：中距離 */
			if (m_distance <= 85.0f)
				candidates[count++] = NPCSwordPattern::enPush;

			/* Air：やや離れているとき */
			if (m_distance >= 50.0f && m_distance <= 95.0f)
				candidates[count++] = NPCSwordPattern::enAir;

			if (count == 0)
				candidates[count++] = NPCSwordPattern::enRush;

			m_currentPattern = candidates[rand() % count];
		}
	}
}