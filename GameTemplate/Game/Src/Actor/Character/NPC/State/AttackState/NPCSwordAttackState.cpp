#include "stdafx.h"
#include "NPCSwordAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	/* 乱数・パターン関連 */
	const auto NUM_SWORD_PATTERNS = 3;             //! 剣NPCが持つ攻撃パターンの総数。
	const auto PATTERN_ZERO = 0;                   //! 乱数のパターン0。
	const auto PATTERN_ONE = 1;                    //! 乱数のパターン1。


	/* 距離・間合い関連 */
	const auto CHASE_TRANSITION_DISTANCE = 100.0f; //! 敵がこの距離より離れたら追跡ステートへ戻る。
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
			/* キャスト。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			m_body = m_brain->GetBody();

			/* 入力システムクラスを取得する。*/
			m_input = &m_body->GetInputClass();

			/* タイマーの初期化。*/
			m_attackTimer = 0;

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
			auto target = m_brain->SearchTarget();

			/* 早期リターン。*/
			if (!target || !m_body) 
				return;

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
			m_diff.Normalize();
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			/* 実行フロー。*/
			ExecutionFlow();

			/* めり込み防止。*/
			PreventClipping(target);

			/* */
			if (m_attackTimer > ATTACK_RESET_TIME)
				Enter();
		}


		void NPCSwordAttackState::Exit() {
			auto body = m_brain->GetBody();
			if (body) {
				body->GetInputClass().SetVirtualButtonB(false);
				body->GetInputClass().SetVirtualController(0.0f, 0.0f);
			}
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
				m_input->SetVirtualButtonB(true);

			/* 2段目の攻撃入力。*/
			if (m_attackTimer == COMBO_SECOND_INPUT)
				m_input->SetVirtualButtonB(true);

			/* 3段目の攻撃入力。*/
			if (m_attackTimer == COMBO_THIRD_INPUT)
				m_input->SetVirtualButtonB(true);
		}


		void NPCSwordAttackState::ExecuteMeleePush()
		{
			/* ダッシュ攻撃の開始入力。*/
			/* LB1を押してダッシュ開始。*/
			/* 1段目の攻撃入力。*/
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_input->SetVirtualButtonA(true);

			/* ダッシュの持続入力。*/
			/* LB1を長押ししてダッシュを持続。*/
			if (m_attackTimer == COMBO_SECOND_INPUT)
				m_input->SetVirtualButtonB(true);
		}


		void NPCSwordAttackState::ExecuteMeleeAir()
		{
			if (m_attackTimer == 1) 
				m_input->SetVirtualButtonA(true);

			if (m_attackTimer == 15) 
				m_input->SetVirtualButtonB(true);
		}


		void NPCSwordAttackState::ResetVirtualInputs() 
		{
			m_input->SetVirtualButtonA(false);    //! Aボタン。
			m_input->SetVirtualButtonB(false);    //! Bボタン。
			m_input->SetVirtualButtonX(false);    //! Xボタン。
			m_input->SetVirtualButtonLB1(false);  //! LB1ボタン。
			m_input->SetVirtualButtonLB2(false);  //! LB2ボタン。
			m_input->SetVirtualButtonRB1(false);  //! RB1ボタン。
			m_input->SetVirtualButtonRT(false);   //! RTボタン。
		}


		void NPCSwordAttackState::PreventClipping(nsActor::Sandbag* target)
		{
			/* 早期リターン。*/
			if (m_distance >= CLIPPING_LIMIT_DISTANCE || m_distance <= 0.0f)
				return;

			/* 距離の計算。*/
			m_pushBackDir = m_body->GetPosition() - target->GetPosition();
			m_pushBackDir.y = 0.0f;
			/* ベクトルの正規化。*/
			m_pushBackDir.Normalize();

			/* めり込んだ分だけ座標を強制的に外側へ押し戻す。*/
			m_currentPosition = m_body->GetPosition();
			m_currentPosition.x += m_pushBackDir.x * (CLIPPING_LIMIT_DISTANCE - m_distance);
			m_currentPosition.z += m_pushBackDir.z * (CLIPPING_LIMIT_DISTANCE - m_distance);

			/* キャラクターコントローラーとプレイヤーの位置を更新。*/
			m_body->GetCharacterController().SetPosition(m_currentPosition);
			m_body->SetPosition(m_currentPosition);
		}


		void NPCSwordAttackState::UpdateFacingDirection()
		{
			if (m_isAttacking) {
				m_body->SetAngle(m_diff.x > 0.0f ? FACING_ANGLE_RIGHT : FACING_ANGLE_LEFT);
				m_body->SetForwardVector(m_diff.x > 0.0f ? Vector3::Right : Vector3::Left);
			}
		}


		void NPCSwordAttackState::UpdateMovement()
		{
			m_stickX = m_isAttacking ? m_diff.x : (m_distance < RETREAT_DISTANCE ? -m_diff.x : 0.0f);
			m_stickZ = m_isAttacking ? m_diff.z : (m_distance < RETREAT_DISTANCE ? -m_diff.z : 0.0f);

			m_input->SetVirtualController(m_stickX, m_stickZ);
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