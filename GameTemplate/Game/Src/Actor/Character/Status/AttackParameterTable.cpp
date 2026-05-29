#include "stdafx.h"
#include "AttackParameterTable.h"

namespace
{
	const auto NORMAL_ATTACK_DAMAGE_RATE = 1.0f;
	const auto PUSH_FORWARD_DAMAGE_RATE = 1.5f;
	const auto CHARGE_ATTACK_DAMAGE_RATE = 2.5f;
	const auto AIR_ATTACK_DAMAGE_RATE = 1.5f;
	const auto RUSH_ATTACK_DAMAGE_RATE = 0.5f;
	const auto MAGIC_ATTACK_DAMAGE_RATE = 1.5f;
}

namespace nsApp
{
	const std::unordered_map<AttackType, AttackParameter> AttackParameterTable::m_table =
	{
		{ AttackType::NormalAttack,     { 1.0f, 0.00f } },
		{ AttackType::ChargeAttack,     { 2.0f, 0.05f } },
		{ AttackType::HeavyAttack,      { 1.8f, 0.05f } },
		{ AttackType::SlashUp,          { 1.2f, 0.03f } },
		{ AttackType::AirAttack,        { 1.25f, 0.03f } },
		{ AttackType::RushAttack_Start, { 0.45f, 0.00f } },
		{ AttackType::RushAttack_End,   { 0.75f, 0.03f } },
		{ AttackType::PushForward,      { 1.1f, 0.03f } },
		{ AttackType::MagicAttack,      { 1.35f, 0.03f } },
		{ AttackType::HeelMagic,        { 0.0f, 0.00f } },
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