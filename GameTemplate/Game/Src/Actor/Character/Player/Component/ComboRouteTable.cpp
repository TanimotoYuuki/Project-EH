#include "stdafx.h"
#include "ComboRouteTable.h"

namespace nsApp
{
	/* エンプティの実態の作成。*/
	const COMBO_RULE ComboRouteTable::m_empty = {};


	const std::unordered_map<PLAYER_STATE_ID, COMBO_RULE> ComboRouteTable::m_groundTable =
	{
		{
			/* パターン1: 通常攻撃から派生。*/
			PLAYER_STATE_ID::enNormalAttack,
			{
			    /* 連続攻撃の開始。*/
				{ ComboInputType::RushB, 10, PLAYER_STATE_ID::enRushStart},
				/* 連続攻撃の終了。*/
				{ ComboInputType::PressB, 15, PLAYER_STATE_ID::enRushEnd},
				/* 重攻撃に派生。*/
		        { ComboInputType::PressX, 10, PLAYER_STATE_ID::enHeavyAttack},
			},
		},
		{
			/* パターン2: 斬り進む攻撃からの派生。*/
			PLAYER_STATE_ID::enPushForward,
			{
			    /* 斬り上げ攻撃への派生。*/
				{ ComboInputType::PressLB2, 10, PLAYER_STATE_ID::enSlashUp },
			},
		},
		{
			/* パターン3: チャージ攻撃からの派生。*/ 
			PLAYER_STATE_ID::enCharging,
			{
			    /* Bボタンを離したらチャージ攻撃状態へ。*/
			    { ComboInputType::PressB, 0, PLAYER_STATE_ID::enChargeAttack},
		    },
		},
	};


	const std::unordered_map<PLAYER_STATE_ID, COMBO_RULE> ComboRouteTable::m_airTable =
	{
		{
			/* パターン3: 斬り上げ攻撃から派生。*/
			PLAYER_STATE_ID::enSlashUp,
			{
			    /* 空中攻撃に派生。*/
				{ ComboInputType::PressB, 10, PLAYER_STATE_ID::enAirAttack }
			},
		},
	};


	const COMBO_RULE& ComboRouteTable::GetRoutes(PLAYER_STATE_ID currentStateID, bool isGrounded)
	{
		/* 地上にいるかどうかでテーブルの種類を区別する。*/
		if (isGrounded)
		{
			/* 地上の場合。*/
			auto it = m_groundTable.find(currentStateID);
			if (it != m_groundTable.end()) 
				return it->second;
		}
		else
		{
			/* 空中の場合。*/
			auto it = m_airTable.find(currentStateID);
			if (it != m_airTable.end()) 
				return it->second;
		}

		return m_empty;
	}
}