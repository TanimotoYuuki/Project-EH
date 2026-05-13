#include "stdafx.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCHammerAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	/* 乱数・パターン関連 */
	const auto NUM_HAMMER_PATTERNS = 3;            //! ハンマーNPCが持つ攻撃パターンの総数

	/* 距離・間合い関連 */
	const auto CHASE_TRANSITION_DISTANCE = 180.0f; //! 敵がこの距離より離れたら追跡ステートへ戻る
	const auto RETREAT_DISTANCE = 50.0f;           //! 攻撃後のクールタイム中に、この距離より近ければ後ろに下がる
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f;    //! めり込みを防止する限界距離（見えない壁）

	/* 時間（フレーム）関連 */
	const auto ATTACK_DURATION = 40;               //! 攻撃モーションが継続し、敵の方を向き続ける時間
	const auto ATTACK_RESET_TIME = 70;             //! 次の行動（再抽選）に移るまでの総時間

	/* コンボ入力のタイミング（フレーム） */
	const auto COMBO_FIRST_INPUT = 1;              //! 1段目のボタン入力タイミング
	const auto COMBO_HEAVY_INPUT = 15;             //! 重攻撃（Xボタン）の入力タイミング
	const auto COMBO_AIR_INPUT = 15;               //! 空中攻撃（Bボタン）の入力タイミング

	const auto PUSH_START_INPUT = 5;               //! ダッシュ攻撃（Push）のBボタン入力タイミング
	const auto PUSH_HOLD_DURATION = 10;            //! ダッシュ攻撃時のLB1（ダッシュ）ボタン長押し時間

	/* モデルの回転角度 */
	const auto FACING_ANGLE_RIGHT = 90.0f;         //! 右を向く時の角度
	const auto FACING_ANGLE_LEFT = -90.0f;         //! 左を向く時の角度
}

namespace nsApp
{
	namespace nsState
	{
		void NPCHammerAttackState::Enter()
		{
			/* キャストとキャッシュの取得 */
			NPCAttackBaseState::Enter();

			/* 攻撃パターンの抽選 */
			m_randomPattern = rand() % NUM_HAMMER_PATTERNS;
			m_currentPattern =
				(m_randomPattern == 0) ? NPCHammerPattern::enHeavy :
				(m_randomPattern == 1) ? NPCHammerPattern::enPush :
				NPCHammerPattern::enAir;
		}


		void NPCHammerAttackState::Update()
		{
			auto target = m_npcBrain->SearchTarget();

			/* 早期リターン */
			if (!target || !m_getBody)
				return;

			/* 距離を計算 */
			ComputeDistance(target);

			/* 一定の距離以上離れた場合、追跡状態に遷移 */
			if (m_distance > CHASE_TRANSITION_DISTANCE) {
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* タイマーの更新 */
			m_attackTimer++;

			/* ベクトルの正規化と攻撃状態の判定 */
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			/* 実行フロー */
			ExecutionFlow();

			/* めり込み防止 */
			PreventClipping(target);

			/* ステートの再抽選 */
			if (m_attackTimer > ATTACK_RESET_TIME)
				Enter();
		}


		void NPCHammerAttackState::ExecuteCurrentCombo()
		{
			/* コンボがない場合 */
			if (m_currentPattern == NPCHammerPattern::enNone)
				return;

			/* 関数ポインタによるディスパッチ */
			void (NPCHammerAttackState:: * actions[])() = {
				&NPCHammerAttackState::ExecuteMeleeHeavy,
				&NPCHammerAttackState::ExecuteMeleePush,
				&NPCHammerAttackState::ExecuteMeleeAir
			};
			(this->*actions[static_cast<int>(m_currentPattern)])();
		}


		void NPCHammerAttackState::ExecuteMeleeHeavy()
		{
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->SetButton(enButtonB, true);

			if (m_attackTimer == COMBO_HEAVY_INPUT)
				m_virtualInput->SetButton(enButtonX,true);
		}


		void NPCHammerAttackState::ExecuteMeleePush()
		{
			if (m_attackTimer < PUSH_HOLD_DURATION)
				m_virtualInput->SetButton(enButtonLB1 ,true);

			if (m_attackTimer == PUSH_START_INPUT)
				m_virtualInput->SetButton(enButtonB ,true);
		}


		void NPCHammerAttackState::ExecuteMeleeAir()
		{
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->SetButton(enButtonA ,true);

			if (m_attackTimer == COMBO_AIR_INPUT)
				m_virtualInput->SetButton(enButtonB ,true);
		}


		void NPCHammerAttackState::UpdateMovement()
		{
			/* ハンマーの場合、Push(ダッシュ)以外はスティックを倒さない（立ち止まる）ようにすると更にハンマーらしくなります */
			m_isDashAttack = (m_currentPattern == NPCHammerPattern::enPush);

			/* 攻撃中はダッシュの時だけスティックを前へ。それ以外（通常・空中）はその場で振る */
			m_stickX = m_isAttacking ? (m_isDashAttack ? m_diff.x : 0.0f) : (m_distance < RETREAT_DISTANCE ? -m_diff.x : 0.0f);
			m_stickZ = m_isAttacking ? (m_isDashAttack ? m_diff.z : 0.0f) : (m_distance < RETREAT_DISTANCE ? -m_diff.z : 0.0f);

			m_virtualInput->SetLStick(m_stickX, m_stickZ);
		}


		void NPCHammerAttackState::ExecutionFlow()
		{
			/* 入力のリセット */
			ResetVirtualInputs();

			/* 技ボタンの入力 */
			ExecuteCurrentCombo();

			/* 向きの更新 */
			UpdateFacingDirection();

			/* 移動/距離の計算 */
			UpdateMovement();
		}
	}
}