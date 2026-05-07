#include "stdafx.h"
#include "NPCWandAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	/* 距離・間合い関連 */
	const auto CHASE_TRANSITION_DISTANCE = 200.0f; //! 敵がこの距離より離れたら追跡ステートへ戻る（杖は長めに設定）
	const auto RETREAT_DISTANCE = 100.0f;          //! 攻撃後のクールタイム中に、この距離より近ければ後ろに下がる（杖は遠目）
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f;    //! めり込みを防止する限界距離（見えない壁）

	/* 時間（フレーム）関連 */
	const auto ATTACK_DURATION = 40;               //! 攻撃モーションが継続し、敵の方を向き続ける時間
	const auto ATTACK_RESET_TIME = 70;             //! 次の行動（再抽選）に移るまでの総時間

	/* コンボ入力のタイミング（フレーム） */
	const auto COMBO_FIRST_INPUT = 1;              //! 1段目のボタン入力タイミング
	const auto COMBO_AIR_INPUT = 15;               //! 空中攻撃（Bボタン）の入力タイミング

	/* モデルの回転角度 */
	const auto FACING_ANGLE_RIGHT = 90.0f;         //! 右を向く時の角度
	const auto FACING_ANGLE_LEFT = -90.0f;         //! 左を向く時の角度
}

namespace nsApp
{
	namespace nsState
	{
		void NPCWandAttackState::Enter()
		{
			/* キャストとキャッシュの取得。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			m_body = m_brain->GetBody();
			m_input = &m_body->GetInputClass();

			/* タイマーの初期化。*/
			m_attackTimer = 0;

			/* 思考ロジック：HPが半分以下なら回復魔法、それ以外は魔法か空中攻撃 */
			if (m_body->GetCharacterStatus().hp.currentHP <= (m_body->GetCharacterStatus().hp.maxHP / 2))
				m_currentPattern = NPCWandPattern::enMagicHeal;

			else
			{
				m_randomPattern = rand() % 2;
				m_currentPattern = (m_randomPattern == 0) ? NPCWandPattern::enMagicAttack : NPCWandPattern::enAir;
			}
		}


		void NPCWandAttackState::Update()
		{
			auto target = m_brain->SearchTarget();

			/* 早期リターン。*/
			if (!target || !m_body)
				return;

			/* 距離を計算。*/
			ComputeDistance(target);

			/* 一定の距離以上離れた場合、追跡状態に遷移（杖は遠め）*/
			if (m_distance > CHASE_TRANSITION_DISTANCE) {
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* タイマーの更新。*/
			m_attackTimer++;

			/* ベクトルの正規化と攻撃状態の判定。*/
			m_diff.Normalize();
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			/* 実行フロー。*/
			ExecutionFlow();

			/* めり込み防止。*/
			PreventClipping(target);

			/* ステートの再抽選。*/
			if (m_attackTimer > ATTACK_RESET_TIME)
				Enter();
		}


		void NPCWandAttackState::Exit()
		{
			if (m_input) {
				m_input->SetVirtualButtonB(false);
				m_input->SetVirtualController(0.0f, 0.0f);
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
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_input->SetVirtualButtonRB1(true);
		}


		void NPCWandAttackState::ExecuteMagicHeal()
		{
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_input->SetVirtualButtonRT(true);
		}


		void NPCWandAttackState::ExecuteMeleeAir()
		{
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_input->SetVirtualButtonA(true);

			if (m_attackTimer == COMBO_AIR_INPUT)
				m_input->SetVirtualButtonB(true);
		}


		void NPCWandAttackState::ResetVirtualInputs()
		{
			m_input->SetVirtualButtonA(false);    //! Aボタン。
			m_input->SetVirtualButtonB(false);    //! Bボタン。
			m_input->SetVirtualButtonX(false);    //! Xボタン。
			m_input->SetVirtualButtonLB1(false);  //! LB1ボタン。
			m_input->SetVirtualButtonLB2(false);  //! LB2ボタン。
			m_input->SetVirtualButtonRB1(false);  //! RB1ボタン。
			m_input->SetVirtualButtonRT(false);   //! RTボタン。
		}


		void NPCWandAttackState::PreventClipping(nsActor::Sandbag* target)
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


		void NPCWandAttackState::UpdateFacingDirection()
		{
			if (m_isAttacking) {
				m_body->SetAngle(m_diff.x > 0.0f ? FACING_ANGLE_RIGHT : FACING_ANGLE_LEFT);
				m_body->SetForwardVector(m_diff.x > 0.0f ? Vector3::Right : Vector3::Left);
			}
		}


		void NPCWandAttackState::UpdateMovement()
		{
			/* 杖は遠距離職なので、攻撃中は立ち止まる（スティック入力0）*/
			m_stickX = m_isAttacking ? 0.0f : (m_distance < RETREAT_DISTANCE ? -m_diff.x : 0.0f);
			m_stickZ = m_isAttacking ? 0.0f : (m_distance < RETREAT_DISTANCE ? -m_diff.z : 0.0f);

			m_input->SetVirtualController(m_stickX, m_stickZ);
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
	}
}