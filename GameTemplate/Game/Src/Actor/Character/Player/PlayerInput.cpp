#include "stdafx.h"
#include "PlayerInput.h"

namespace
{
	const auto CHARGE_ATTACK_DETECTION = 30.0f; /* チャージ攻撃の判定時間。*/
	const auto CHARGE_FLAG_TRUE = 1;
	const auto CHARGE_FLAG_FALSE = 0;
}

/** @def
 *  RTボタン判定。
 *  エンジン内でのボタン判定はRT2だが、XboxコントローラーだとRTに該当し、視認性が悪いため定義する。
 */
#define BUTTON_RT enButtonRB2

namespace nsApp
{
	void PlayerInput::Update()
	{
		/* Bボタンを押しているかを取得。*/
		m_isPressButton = g_pad[m_padInddex]->IsPress(enButtonB);

		/* 入力判定。*/
		if (!m_isInputEnable)
		{
			m_isAttack = false;          /* 攻撃フラグ。*/
			m_isMove = false;            /* 移動フラグ。*/
			m_isJump = false;            /* ジャンプフラグ。*/
			m_isRun = false;             /* 走りフラグ。*/
			m_isDamage = false;          /* ダメージフラグ。*/
			m_isDeath = false;           /* 死亡フラグ。*/
			m_isNormalAttack = false;    /* 通常攻撃フラグ。*/
			m_isChargeAttack = false;    /* チャージ攻撃フラグ。*/
			m_isAirAttack = false;       /* 空中攻撃フラグ。*/
			m_isComboAttack = false;     /* コンボ攻撃フラグ。*/
			m_isRushStart = false;       /* 連続攻撃開始フラグ。*/
			m_isRushEnd = false;         /* 連続攻撃終了フラグ。*/
			m_moveVec = Vector3::Zero;   
			return;
		}

		/* 移動入力判定。*/ 
		m_stickX = g_pad[m_padInddex]->GetLStickXF();
		m_stickY = g_pad[m_padInddex]->GetLStickYF();

		/* ジャンプ, 斬り上げ判定。*/
		if (g_pad[m_padInddex]->IsTrigger(enButtonA))
		{
			if (m_stickY > 0.5f)
			{
				m_isSlashUp = true;
				m_isJump = false;
			}

			else
			{
				m_isJump = true;
				m_isSlashUp = false;
			}
		}

		/* Aボタンが押されていないならフラグを変えない。*/
		else
		{
			m_isJump = false;
			m_isSlashUp = false;
		}

		/* 斬り上げ判定。*/
		m_isSlashUp = g_pad[m_padInddex]->IsTrigger(enButtonLB2);

		/* ジャンプ判定。*/
		m_isJump = g_pad[m_padInddex]->IsTrigger(enButtonA);

		/* Bボタンを押した瞬間の攻撃判定を設定。*/
		m_isAttack = g_pad[m_padInddex]->IsTrigger(enButtonB);

		/* カメラを考慮せずにとりあえずスティックの入力量で移動する。*/
		m_moveVec = Vector3(m_stickX, 0.0, 0.0);

		/* 少量でもスティックの移動量があるなら移動中とする。*/
		m_isMove = (m_moveVec.Length() > 0.1f);

		/* ダメージ判定。*/
		/* ※ボスが実装されるまで仮置き。*/
		m_isDamage = g_pad[m_padInddex]->IsTrigger(enButtonLeft);

		/* 死亡判定。*/
		/* ※テストでYボタン判定とする。*/
		m_isDeath = g_pad[m_padInddex]->IsTrigger(enButtonDown);

		/* スティックの押し具合は厳しいので特定のボタン同士で走れるようにする。*/
		m_isRun = (g_pad[m_padInddex]->IsPress(enButtonLB1) && m_isMove);


		/* 攻撃入力判定。*/

        /* 通常攻撃。 
		 * 30F 押しているかを判定とする。
		 */
		m_isNormalAttack = (!m_isPressButton && m_chargeButtonTimer > 0 && m_chargeButtonTimer < CHARGE_ATTACK_DETECTION);

		/* チャージ攻撃。
		* 30F以上(長押し) Bボタンを押しているかを判定。
		*/
		m_isChargeAttack = (!m_isPressButton && m_chargeButtonTimer >= CHARGE_ATTACK_DETECTION);

		/* 空中攻撃。
		* 空中でBボタンを押しているかを判定。
		*/
		m_isAirAttack = g_pad[m_padInddex]->IsTrigger(enButtonB);

		/* タイマー処理。
		* 押せば加算。
		* 離せばリセット。
		*/
		m_chargeButtonTimer = m_isPressButton ? m_chargeButtonTimer + CHARGE_FLAG_TRUE : CHARGE_FLAG_FALSE;

		/* チャージ攻撃開始。
		 * 5F以上(長押し) Bボタンを押しているかを判定。
		 */ 
		m_isChargeStart = (m_isPressButton && m_chargeButtonTimer >= 12);


		/* ガード判定。*/ 
		m_isGuard = g_pad[m_padInddex]->IsPress(enButtonLB2);

		/* 助ける判定。*/ 
		m_isHelp = g_pad[m_padInddex]->IsTrigger(enButtonY);

		/* 重攻撃判定。*/
		m_isPressX = g_pad[m_padInddex]->IsTrigger(enButtonX);

		/* RBボタン判定。*/
		m_isPressRB = g_pad[m_padInddex]->IsPress(enButtonRB1);

		/* RTボタン判定。*/
		m_isPressRT = g_pad[m_padInddex]->IsPress(BUTTON_RT);
	}
}