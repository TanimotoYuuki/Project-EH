#include "stdafx.h"
#include "NPCWandAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCAttackBaseState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace
{
	/* 乱数関連。*/
	const auto RANDOM_PATTERN_0 = 0;               //! 乱数のシード値。
	const auto DIVISOR = 2;                        //! 乱数の分母。

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
			/* キャストとキャッシュの取得。*/
			NPCAttackBaseState::Enter();

			/* 思考ロジック：HPが半分以下なら回復魔法、それ以外は魔法か空中攻撃 */
			if (m_getBody->GetCharacterStatus().hp.currentHP <= (m_getBody->GetCharacterStatus().hp.maxHP / 2))
				m_currentPattern = NPCWandPattern::enMagicHeal;

			else
			{
				/* 乱数で攻撃パターンを決定。*/
				m_randomPattern = rand() % DIVISOR;
				/* 乱数の値に応じて攻撃パターンを設定。*/
				m_currentPattern = (m_randomPattern == RANDOM_PATTERN_0) ? NPCWandPattern::enMagicAttack : NPCWandPattern::enAir;
			}
		}


		void NPCWandAttackState::Update()
		{
			/* ヘルプ遷移の判定。*/
			if (CheckHelpTransition())
				return;

			/* ターゲットの検索。*/
			nsActor::ICharacter* target = m_npcBrain->SearchTarget();

			/* 早期リターン。*/
			if (target == nullptr || m_getBody == nullptr)
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
			m_isAttacking = (m_attackTimer < ATTACK_DURATION);

			/* 実行フロー。*/
			ExecutionFlow();

			/* めり込み防止。*/
			PreventClipping(target);

			/* 通常終了。アニメーションが終わっていれば安全に待機へ戻る。*/
			if (m_attackTimer > ATTACK_RESET_TIME && !m_getBody->IsPlayAnimation())
			{
				/* 入力値のリセット。*/
				if (m_virtualInput != nullptr)
					m_virtualInput->Reset();

				/* 待機状態へ遷移。*/
				m_stateMachine->ChangeState(new NPCIdleState());
				return;
			}

			/* 保険。アニメーション終了判定が戻らない場合でも永久停止しないようにする。*/
			if (m_attackTimer > ATTACK_FORCE_RESET_TIME)
			{
				/* 入力値のリセット。*/
				if (m_virtualInput != nullptr)
					m_virtualInput->Reset();

				/* 待機状態へ遷移。*/
				m_stateMachine->ChangeState(new NPCIdleState());
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
			/* 攻撃の最初のフレームでRB2を押す。*/
			if (m_attackTimer == COMBO_FIRST_INPUT)
				m_virtualInput->RequestButton(enButtonRB2, HOLD_FRAME);
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
			/* 杖は遠距離職なので、攻撃中は立ち止まる（スティック入力0）*/
			m_stickX = m_isAttacking ? STICK_NEUTRAL : (m_distance < RETREAT_DISTANCE ? -m_diff.x : STICK_NEUTRAL);
			m_stickZ = m_isAttacking ? STICK_NEUTRAL : (m_distance < RETREAT_DISTANCE ? -m_diff.z : STICK_NEUTRAL);

			/* 入力値をセットする。*/
			m_virtualInput->SetLStick(m_stickX, m_stickZ);
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