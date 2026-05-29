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
		/* 入力判定。*/
		if (!m_isInputEnable || !m_inputDevice)
		{
			InitInputJudgment();
			return;
		}

		/* 移動入力判定。*/
		m_stickX = m_inputDevice->GetLStickX();
		m_stickY = m_inputDevice->GetLStickY();

		/* スティック移動量の計算 */
		m_moveVec = Vector3(m_stickX, 0.0f, 0.0f);

		if (m_moveVec.LengthSq() > 0.1f)
			m_moveVec.Normalize();

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
	}


	bool PlayerInput::CheckButtonPress(nsK2EngineLow::EnButton inputButtonType)
	{
		if (!m_inputDevice)
			return false;

		/* コントローラーで操作する場合はgamePadを読み込む。*/
		return m_inputDevice->IsPress(inputButtonType);
	}


	bool PlayerInput::CheckButtonTrigger(nsK2EngineLow::EnButton inputButtonType)
	{
		if (!m_inputDevice)
			return false;

		return m_inputDevice->IsTrigger(inputButtonType);
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
		m_isNormalAttack = false;    //! 通常攻撃フラグ。
		m_isChargeAttack = false;    //! チャージ攻撃フラグ。
		m_isAirAttack = false;       //! 空中攻撃フラグ。
		m_isComboAttack = false;     //! コンボ攻撃フラグ。
		m_isRushStart = false;       //! 連続攻撃開始フラグ。
		m_isRushEnd = false;         //! 連続攻撃終了フラグ。
		m_moveVec = Vector3::Zero;
	}
}