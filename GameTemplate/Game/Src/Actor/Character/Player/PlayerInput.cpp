#include "stdafx.h"
#include "PlayerInput.h"

namespace nsApp
{
	void PlayerInput::Update()
	{
		/* 入力判定。*/
		if (!m_isInputEnable)
		{
			m_isAttack = false;          /* 攻撃フラグ。*/
			m_isMove = false;            /* 移動フラグ。*/
			m_isJump = false;            /* ジャンプフラグ。*/
			m_isRun = false;             /* 走りフラグ。*/
			m_isDamage = false;          /* ダメージフラグ。*/
			m_isDeath = false;           /* 死亡フラグ。*/
			m_moveVec = Vector3::Zero;   
			return;
		}

		/* 攻撃入力判定。*/
		m_isAttack = g_pad[0]->IsTrigger(enButtonB);

		/* ジャンプ判定。*/
		m_isJump = g_pad[0]->IsTrigger(enButtonA);

		/* 移動入力判定。*/ 
		m_stickX = g_pad[0]->GetLStickXF();
		m_stickY = g_pad[0]->GetLStickYF();

		/* カメラを考慮せずにとりあえずスティックの入力量で移動する。*/
		m_moveVec = Vector3(m_stickX, 0.0, 0.0);

		/* 少量でもスティックの移動量があるなら移動中とする。*/
		m_isMove = (m_moveVec.Length() > 0.1f);

		/* ダメージ判定。*/
		/* ※ボスが実装されるまで仮置き。*/
		m_isDamage = g_pad[0]->IsTrigger(enButtonX);

		/* 死亡判定。*/
		/* ※テストでYボタン判定とする。*/
		m_isDeath = g_pad[0]->IsTrigger(enButtonY);

		/* スティックの押し具合は厳しいので特定のボタン同士で走れるようにする。*/
		m_isRun = (g_pad[0]->IsPress(enButtonLB1) && m_isMove);
	}
}

