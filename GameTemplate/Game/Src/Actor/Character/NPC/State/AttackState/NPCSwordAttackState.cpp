#include "stdafx.h"
#include "NPCSwordAttackState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"

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

	/* モデルの回転角度 */
	const auto FACING_ANGLE_RIGHT = 90.0f;         //! 右を向く時の角度。
	const auto FACING_ANGLE_LEFT = -90.0f;         //! 左を向く時の角度。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCSwordAttackState::Enter()
		{
			/* 親クラスで初期化を行う。*/
			NPCAttackBaseState::Enter();

			/* 乱数の初期化。*/
			m_randomPattern = rand() % NUM_SWORD_PATTERNS;

			/* 攻撃パターンの初期化。*/
			m_currentPattern = 
				(m_randomPattern == PATTERN_ZERO) ? NPCSwordPattern::enAir :
				(m_randomPattern == PATTERN_ONE) ? NPCSwordPattern::enPush :
				NPCSwordPattern::enRush;
		}


		void NPCSwordAttackState::Update()
		{
			if (CheckHelpTransition())
				return;

			/* 目標を探索する。*/
			nsActor::ICharacter* target = m_npcBrain->SearchTarget();

			/* 早期リターン。*/
			if (target == nullptr || m_getBody == nullptr)
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* 距離を計算。*/
			ComputeDistance(target);

			/* 一定の距離の場合、待機状態に遷移。*/
			if (m_distance > CHASE_TRANSITION_DISTANCE) {
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}
			/* タイマーの更新。*/
			m_attackTimer++;

			/* ベクトルの正規化。*/
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			/* 実行フロー。*/
			ExecutionFlow();

			/* めり込み防止。*/
			PreventClipping(target);

			/* */
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
				m_virtualInput->RequestButton(enButtonB,3);

			/* 2段目の攻撃入力。*/
			if (m_attackTimer == COMBO_SECOND_INPUT)
				m_virtualInput->RequestButton(enButtonB,3);

			/* 3段目の攻撃入力。*/
			if (m_attackTimer == COMBO_THIRD_INPUT)
				m_virtualInput->RequestButton(enButtonB,3);
		}


		void NPCSwordAttackState::ExecuteMeleePush()
		{
			/* ダッシュ攻撃の開始入力。*/
			/* LB1を押してダッシュ開始。*/
			/* 1段目の攻撃入力。*/
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonA,3);

			/* ダッシュの持続入力。*/
			/* LB1を長押ししてダッシュを持続。*/
			if (m_attackTimer == COMBO_SECOND_INPUT)
				m_virtualInput->RequestButton(enButtonB,3);
		}


		void NPCSwordAttackState::ExecuteMeleeAir()
		{
			if (m_attackTimer == 1) 
				m_virtualInput->RequestButton(enButtonA,3);

			if (m_attackTimer == 15) 
				m_virtualInput->RequestButton(enButtonB,3);
		}


		void NPCSwordAttackState::UpdateMovement()
		{
			m_stickX = m_isAttacking ? 0.0f : (m_distance < RETREAT_DISTANCE ? -m_diff.x : 0.0f);
			m_stickZ = m_isAttacking ? 0.0f : (m_distance < RETREAT_DISTANCE ? -m_diff.z : 0.0f);

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
	}
}