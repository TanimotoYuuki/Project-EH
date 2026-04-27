#include "stdafx.h"
#include "PlayerInput.h"

namespace
{
	const auto CHARGE_ATTACK_DETECTION = 30.0f; //! チャージ攻撃の判定時間。
	const auto CHARGE_FLAG_TRUE = 1;
	const auto CHARGE_FLAG_FALSE = 0;
	const auto PAD_INDEX_NAM = 0;
}


namespace nsApp
{
	void PlayerInput::Update()
	{
		/* 入力判定が無効な場合は初期化して帰る */
		if (!m_isInputEnable)
		{
			InitInputJudgment();
			return;
		}

		if (m_padInddex < PAD_INDEX_NAM)
		{
			m_stickX = m_virtualStickX;
			m_stickY = m_virtualStickY;
		}
		else
		{
			m_stickX = g_pad[m_padInddex]->GetLStickXF();
			m_stickY = g_pad[m_padInddex]->GetLStickYF();
		}

		/* スティック移動量の計算 */
		m_moveVec = Vector3(m_stickX, 0.0f, m_stickY);
		m_isMove = (m_moveVec.Length() > 0.1f);

		/* Bボタン。*/
		SummarizeButtonB();

		/* ジャンプと斬り上げ判定の更新。*/
		EvaluateJumpAndSlashUp();

		/* その他ボタン判定 */
		SummarizeOtherButtons();

		/* 走り判定 */
		m_isRun = (CheckButtonPress(enButtonLB1) && m_isMove);

		/* チャージ判定を更新する。*/
		UpdateChargeTranslation();


		/* AIControllerの前フレームのボタンの情報を保持する。*/
		if (m_padInddex < PAD_INDEX_NAM)
			SetVirtualAttackButtons();
	}


	bool PlayerInput::CheckButtonPress(nsK2EngineLow::EnButton inputButtonType)
	{
		/* NPCの場合は仮想コントローラーフラグを返す。*/
		if (m_padInddex < PAD_INDEX_NAM)
		{
			switch (inputButtonType)
			{
			case enButtonA: 
				return m_isVirtualPressA;

			case enButtonB:
				return m_isVirtualPressB;

			case enButtonX:
				return m_isVirtualPressX;

			case enButtonY:
				return m_isVirtualPressY;

			case enButtonLB1:
				return m_isVirtualPressLB1;

			case enButtonLB2:
				return m_isVirtualPressLB2;

			case enButtonRB1:
				return m_isVirtualPressRB1;	

			case enButtonRB2:
				return m_isVirtualPressRT;

			default:
				return false;
			}
		}

		/* コントローラーで操作する場合はgamePadを読み込む。*/
		return g_pad[m_padInddex]->IsPress(inputButtonType);
	}


	bool PlayerInput::CheckButtonTrigger(nsK2EngineLow::EnButton inputButtonType)
	{
		/* NPCの場合かつ前フレームは押されていなかった時にtrue */
		if (m_padInddex < PAD_INDEX_NAM)
		{
			switch (inputButtonType)
			{
			case enButtonA:   
				return m_isVirtualPressA && !m_prevVirtualPressA;

			case enButtonB:   
				return m_isVirtualPressB && !m_prevVirtualPressB;

			case enButtonX:   
				return m_isVirtualPressX && !m_prevVirtualPressX;

			case enButtonY:   
				return m_isVirtualPressY && !m_prevVirtualPressY;

			case enButtonLB1: 
				return m_isVirtualPressLB1 && !m_prevVirtualPressLB1;

			case enButtonLB2: 
				return m_isVirtualPressLB2 && !m_prevVirtualPressLB2;

			case enButtonRB1: 
				return m_isVirtualPressRB1 && !m_prevVirtualPressRB1;

			case enButtonRB2: 
				return m_isVirtualPressRT && !m_prevVirtualPressRT;

			default:
				return false;
			}
		}
		return g_pad[m_padInddex]->IsTrigger(inputButtonType);
	}


	void PlayerInput::EvaluateJumpAndSlashUp()
	{
		SetJumpFlag(false);
		SetSlashUpFlag(false);

		if (CheckButtonTrigger(enButtonA))
		{
			if (m_stickY > 0.5f)
				SetSlashUpFlag(true);
			else
				SetJumpFlag(true);
		}

		if(CheckButtonTrigger(enButtonLB2))
			SetSlashUpFlag(true);
	}


	void PlayerInput::InitInputJudgment()
	{
		m_isAttack = false;          //! 攻撃フラグ。
		m_isMove = false;            //! 移動フラグ。
		m_isJump = false;            //! ジャンプフラグ。
		m_isRun = false;             //! 走りフラグ。
		m_isDamage = false;          //! ダメージフラグ。
		m_isDeath = false;           //! 死亡フラグ。
		m_isNormalAttack = false;    //! 通常攻撃フラグ。
		m_isChargeAttack = false;    //! チャージ攻撃フラグ。
		m_isAirAttack = false;       //! 空中攻撃フラグ。
		m_isComboAttack = false;     //! コンボ攻撃フラグ。
		m_isRushStart = false;       //! 連続攻撃開始フラグ。
		m_isRushEnd = false;         //! 連続攻撃終了フラグ。
		m_moveVec = Vector3::Zero;
	}
}