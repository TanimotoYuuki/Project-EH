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
				{ ComboInputType::RushB, 20, PLAYER_STATE_ID::enRushStart},
				{ ComboInputType::PressB, 15, PLAYER_STATE_ID::enRushEnd},
			},
		},
		{
			/* パターン2: 斬り進む攻撃からの派生。*/
			PLAYER_STATE_ID::enPushForward,
			{
				{ ComboInputType::PressLB2, 10, PLAYER_STATE_ID::enSlashUp },
			},
		},
	};


	const std::unordered_map<PLAYER_STATE_ID, COMBO_RULE> ComboRouteTable::m_airTable =
	{
		{
			/* パターン3: 斬り上げ攻撃から派生。*/
			PLAYER_STATE_ID::enSlashUp,
			{
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