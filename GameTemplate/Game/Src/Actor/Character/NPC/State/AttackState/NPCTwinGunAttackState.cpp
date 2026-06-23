#include "stdafx.h"
#include "NPCTwinGunAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/Movement/NPCCombatRangeHelper.h"
#include "Src/Actor/Character/NPC/Movement/NPCMovementController.h"

namespace
{
	/* 乱数・パターン関連 */
	const auto NUM_TWINGUN_PATTERNS = 3;           //! 双銃NPCが持つ攻撃パターンの総数
	const auto RANDOM_SEED_0 = 0;			       //! 乱数シード。
	const auto RANDOM_SEED_1 = 1;				   //! 乱数シード。

	/* 距離・間合い関連 */
	const auto RETREAT_DISTANCE = 80.0f;           //! 攻撃後のクールタイム中に、近ければ後ろに下がる
	const auto CLIPPING_LIMIT_DISTANCE = 40.0f;    //! めり込みを防止する限界距離
	const auto RUSH_MIN_DISTANCE = 40.0f;          //! これより近いと Rush 候補にしない（後退ライン + 余裕）
	const auto AIR_MIN_DISTANCE = 60.0f;           //! これより近いと Air 候補にしない

	/* 時間（フレーム）関連 */
	const auto ATTACK_DURATION = 40;               //! 攻撃モーションが継続し、敵の方を向き続ける時間
	const auto ATTACK_RESET_TIME = 70;             //! 次の行動（再抽選）に移るまでの総時間

	/* コンボ入力のタイミング（フレーム） */
	const auto COMBO_FIRST_INPUT = 1;              //! 1段目のボタン入力タイミング
	const auto COMBO_SECOND_INPUT = 10;            //! B連打用
	const auto COMBO_THIRD_INPUT = 20;             //! B連打用
	const auto COMBO_AIR_INPUT = 15;               //! 空中攻撃（Bボタン）の入力タイミング
	const auto HOLD_FRAME = 3;					   //! ボタンを押し続けるフレーム数。
	const auto RETREAT_RANGE_MARGIN = 50.0f;	   //! 理想距離からこれだけ近いと後退する距離。
	const auto RANGED_ATTACK_RANGE = 250.0f;	   //! これ以上なら遠距離扱い（後退あり）。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCTwinGunAttackState::ChoosePattern()
		{
			const NPCBehaviorProfile& profile = m_npcBrain->GetBehaviorProfile();
			const float retreatDist = nsNPC::GetRetreatDistanceByProfile(profile);

			NPCTwinGunPattern candidates[3];
			int count = 0;

			/* Heavy は常に候補（攻撃帯にいる限り使える） */
			candidates[count++] = NPCTwinGunPattern::enHeavy;

			/* Rush：ボス硬直中 & 十分な距離があるとき */
			if (m_distance >= retreatDist + RUSH_MIN_DISTANCE && m_npcBrain->IsBossAttackWindow())
				candidates[count++] = NPCTwinGunPattern::enRush;

			/* Air：十分な距離があるとき */
			if (m_distance >= retreatDist + AIR_MIN_DISTANCE)
				candidates[count++] = NPCTwinGunPattern::enAir;

			m_currentPattern = candidates[rand() % count];
		}


		void NPCTwinGunAttackState::Enter()
		{
			/* 攻撃タイマーリセット */
			NPCAttackBaseState::Enter();

			/* 攻撃パターンは距離計算後の Update で選択する */
		}


		void NPCTwinGunAttackState::Update()
		{
			/* まずはヘルプ遷移の条件をチェック。*/
			if (CheckHelpTransition())
				return;

			/* 危険回避が必要かチェック。*/
			if (CheckEvadeTransition())
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

			const NPCBehaviorProfile& profile = m_npcBrain->GetBehaviorProfile();

			/* 近すぎたらその場で後退（Chase 往復で横跳びしない） */
			if (nsNPC::ShouldRetreatWithHysteresis(m_distance, profile, m_npcBrain->GetIsRetreatingFlag()))
			{
				UpdateFacingDirection();
				UpdateMovement();  // 後退だけ、Chase には戻さない
				m_attackTimer++;
				return;
			}

			/* 一定の距離以上離れた場合、追跡ステートへ戻る（プロファイル基準） */
			const float chaseReturn = nsNPC::GetRangedChaseEnterDistance(profile) - 5.0f;
			if (m_distance > chaseReturn)
			{
				m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}


			/* 攻撃サイクル開始時にパターンを選択 */
			if (m_attackTimer == 0)
				ChoosePattern();

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
				m_virtualInput->RequestButton(enButtonB, 3);
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
			if (m_virtualInput == nullptr || m_npcBrain == nullptr)
				return;

			const NPCBehaviorProfile& profile = m_npcBrain->GetBehaviorProfile();

			if (nsNPC::ShouldRetreatWithHysteresis(m_distance, profile, m_npcBrain->GetIsRetreatingFlag()))
			{
				const Vector3 myPos = m_getBody->GetPosition();
				const Vector3 targetPos = myPos + m_diff * m_distance;
				const Vector3 away = nsNPC::CalcRetreatDirection(myPos, targetPos);

				const NPCMovementIntent intent = NPCMovementController::MakeMoveIntent(away, true);
				NPCMovementController::Apply(m_virtualInput, intent);
				return;
			}

			NPCMovementController::Stop(m_virtualInput);
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