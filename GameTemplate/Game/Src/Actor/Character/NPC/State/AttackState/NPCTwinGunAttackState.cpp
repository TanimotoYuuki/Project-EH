#include "stdafx.h"
#include "NPCTwinGunAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace
{
	/* 乱数・パターン関連 */
	const auto NUM_TWINGUN_PATTERNS = 3;           //! 双銃NPCが持つ攻撃パターンの総数
	const auto RANDOM_SEED_0 = 0;			       //! 乱数シード。
	const auto RANDOM_SEED_1 = 1;				   //! 乱数シード。

	/* 距離・間合い関連 */
	const auto CHASE_TRANSITION_DISTANCE = 300.0f; //! 敵がこの距離より離れたら追跡ステートへ戻る
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

	const auto HOLD_FRAME = 3;					   //! ボタンを押し続けるフレーム数。
	const auto STICK_NEUTRAL = 0.0f;			   //! スティックをニュートラル値。

}

namespace nsApp
{
	namespace nsState
	{
		void NPCTwinGunAttackState::Enter()
		{
			/* 攻撃タイマーリセット */
			NPCAttackBaseState::Enter();

			/* 攻撃パターンをランダムに選択 */
			m_randomPattern = rand() % NUM_TWINGUN_PATTERNS;
			m_currentPattern =
				(m_randomPattern == RANDOM_SEED_0) ? NPCTwinGunPattern::enRush :
				(m_randomPattern == RANDOM_SEED_1) ? NPCTwinGunPattern::enHeavy :
				NPCTwinGunPattern::enAir;
		}


		void NPCTwinGunAttackState::Update()
		{
			/* まずはヘルプ遷移の条件をチェック。*/
			if (CheckHelpTransition())
				return;

			/* ターゲットを検索。*/
			nsActor::ICharacter* target = m_npcBrain->SearchTarget();

			/* ターゲットがいない、もしくはターゲットの体が取得できない場合は追跡ステートへ遷移。*/
			if (target == nullptr || m_getBody == nullptr)
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}
				
			/* 距離を計算。*/
			ComputeDistance(target);

			/* 一定の距離感になった場合は再び追跡ステートに遷移する。*/
			if (m_distance > CHASE_TRANSITION_DISTANCE) {
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* 攻撃タイマーを更新する。*/
			m_attackTimer++;

			/* ベクトルを正規化する。*/
			m_diff.Normalize();
			/* 攻撃モーション中は敵の方を向き続ける */
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			/* 攻撃の実行。*/
			ExecutionFlow();

			/* めり込み防止処理 */
			PreventClipping(target);

			/* 攻撃モーションが終わり、次の行動に移る時間になったら再度攻撃ステートへ遷移する。*/
			if (m_attackTimer > ATTACK_RESET_TIME)
				Enter();
		}


		void NPCTwinGunAttackState::ExecuteCurrentCombo()
		{
			/* どの攻撃パターンにも属さない検知が来た場合、処理を止める。*/
			if (m_currentPattern == NPCTwinGunPattern::enNone)
				return;

			/* 攻撃パターンに応じた攻撃処理を実行する。 */
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
				m_virtualInput->RequestButton(enButtonB,3);
		}


		void NPCTwinGunAttackState::ExecuteShootHeavy()
		{
			/* Xボタンで重攻撃（爆発弾）を撃つ */
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonX, HOLD_FRAME);
		}


		void NPCTwinGunAttackState::ExecuteShootAir()
		{
			/* Aボタンでジャンプし、空中でBを押して射撃 */
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonA, HOLD_FRAME);

			if (m_attackTimer == COMBO_AIR_INPUT)
				m_virtualInput->RequestButton(enButtonB, HOLD_FRAME);
		}


		void NPCTwinGunAttackState::UpdateMovement()
		{
			/* 銃は遠距離なので攻撃中は立ち止まり、近すぎたら下がる */
			m_stickX = m_isAttacking ? STICK_NEUTRAL : (m_distance < RETREAT_DISTANCE ? -m_diff.x : STICK_NEUTRAL);
			m_stickZ = m_isAttacking ? STICK_NEUTRAL : (m_distance < RETREAT_DISTANCE ? -m_diff.z : STICK_NEUTRAL);

			m_virtualInput->SetLStick(m_stickX, m_stickZ);
		}


		void NPCTwinGunAttackState::ExecutionFlow()
		{
			/* 攻撃の実行。*/
			ResetVirtualInputs();

			/* コンボの実行。*/
			ExecuteCurrentCombo();

			/* キャラクターの向きと移動の更新。*/
			UpdateFacingDirection();

			/* 攻撃中の移動の更新。*/
			UpdateMovement();
		}
	}
}