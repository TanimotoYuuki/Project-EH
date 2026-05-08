#include "stdafx.h"
#include "AttackParameterTable.h"

namespace
{
	const auto CONSECUTIVE_ATTACK_RATE = 0.5f;   //! 連続攻撃のダメージ倍率。
	const auto NORMAL_ATTACK_DAMAGE_RATE = 1.0f; //! 通常攻撃のダメージ倍率。
	const auto PUSH_FORWARD_DAMAGE_RATE = 1.0f;  //! 斬り上げ攻撃のダメージ倍率。
	const auto CHARGE_ATTACK_DAMAGE_RATE = 1.5f; //! チャージ攻撃のダメージ倍率。
	const auto AIR_ATTACK_DAMAGE_RATE = 2.0f;    //! バックスラッシュ(空中攻撃)のダメージ倍率。
	const auto MAGIC_ATTACK_DAMAGE_RATE = 1.0f;  //! 魔法攻撃のダメージ倍率。
	const auto HEEL_MAGIC_HEEL_RATE = 0.0f;      //! 回復魔法の回復倍率。
}

namespace nsApp
{
	const std::unordered_map<AttackType, AttackParameter> AttackParameterTable::m_table =
	{
		/* 通常攻撃: ダメージ倍率:等倍。 クリティカル補正:なし。*/
		{AttackType::NormalAttack, {NORMAL_ATTACK_DAMAGE_RATE, 0.0f}},

		/* チャージ攻撃: ダメージ倍率:1.5倍。クリティカル補正:0.2倍。*/
		{AttackType::ChargeAttack, {CHARGE_ATTACK_DAMAGE_RATE, CHARGE_ATTACK_DAMAGE_RATE * 0.2f}},

		/* 重攻撃: ステータスはチャージ攻撃と同じ。*/
		{AttackType::HeavyAttack, {CHARGE_ATTACK_DAMAGE_RATE, CHARGE_ATTACK_DAMAGE_RATE * 0.2f}},

		/* 斬り上げ攻撃: ダメージ倍率:等倍。*/
		{AttackType::SlashUp, {NORMAL_ATTACK_DAMAGE_RATE, NORMAL_ATTACK_DAMAGE_RATE * 0.0f}},

		/* バックスラッシュ(空中攻撃): ダメージ倍率: 2.0f, クリティカル補正:0.1倍。*/
		{AttackType::AirAttack, {AIR_ATTACK_DAMAGE_RATE, AIR_ATTACK_DAMAGE_RATE * 0.1}},

		/* 連続攻撃: 1発は軽い(0.5倍。)*/
		{AttackType::RushAttack_Start, {CONSECUTIVE_ATTACK_RATE, 0.0f}},
		{AttackType::RushAttack_End, {CONSECUTIVE_ATTACK_RATE, 0.0f}},

		/* 突き進み攻撃。*/
		{AttackType::PushForward, {PUSH_FORWARD_DAMAGE_RATE, 0.0f}},

		/* 魔法攻撃。*/
		{AttackType::MagicAttack, {MAGIC_ATTACK_DAMAGE_RATE, 0.0f}},

		/* 回復魔法。*/
		/* @TODO: ダメージ倍率を回復倍率として実装。*/
		{AttackType::HeelMagic, {HEEL_MAGIC_HEEL_RATE, 0.0f}},
	};


	const AttackParameter& AttackParameterTable::GetAttackParameter(AttackType type)
	{
		/* テーブルから該当する攻撃を探索し、戻り値として返す。*/
		auto it = m_table.find(type);
		if (it != m_table.end())
			return it->second;

		return m_table.at(AttackType::NormalAttack);
	}

}