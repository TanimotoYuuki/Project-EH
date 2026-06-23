#include "stdafx.h"
#include "AttackParameterTable.h"
#include "Src/Utilty/TSVTableLoder.h"

namespace nsApp
{
	std::unordered_map<AttackType, AttackParameter> AttackParameterTable::m_table;


	bool AttackParameterTable::LoadTSVFile(const char* filePath)
	{
		/* AttackType列を優先し、空の場合はType列をキーとして使う。 */
		return TSVTableLoder::LoadTable<AttackType, AttackParameter>(
			filePath,
			"AttackType",
			"Type",
			m_table,
			AttackParameterTable::ConvertAttackType,
			[](const TSVTable& table, int rowIndex, AttackType attackType)
			{
				/* 攻撃タイプごとのデフォルト値を使いながら、TSVの1行からAttackParameterを作成する。 */
				return AttackParameterTable::CreateParameterFromRow(table, rowIndex, attackType);
			}
		);
	}


	const AttackParameter& AttackParameterTable::GetAttackParameter(AttackType attackType)
	{
		/* TSVファイルから攻撃パラメータを取得し、存在しない場合はデフォルト値を返す。 */
		auto iterator = m_table.find(attackType);
		if (iterator != m_table.end())
			return iterator->second;

		return GetDefaultParameter(attackType);
	}


	const AttackParameter& AttackParameterTable::GetDefaultParameter(AttackType attackType)
	{
		/* 攻撃タイプごとのデフォルト値を定義する。 */
		static const std::unordered_map<AttackType, AttackParameter> defaultTable =
		{
			{ AttackType::NormalAttack,     { 1.0f,  0.00f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::ChargeAttack,     { 2.0f,  0.05f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::HeavyAttack,      { 1.8f,  0.05f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::SlashUp,          { 1.2f,  0.03f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::AirAttack,        { 1.25f, 0.03f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::RushAttack_Start, { 0.45f, 0.00f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::RushAttack_End,   { 0.75f, 0.03f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::PushForward,      { 1.1f,  0.03f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::MagicAttack,      { 1.35f, 0.03f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } },
			{ AttackType::HeelMagic,        { 0.0f,  0.00f, 8.0f, 80.0f, 40.0f, 8, 0, 10 } }
		};

		auto iterator = defaultTable.find(attackType);
		if (iterator != defaultTable.end())
			return iterator->second;

		return defaultTable.at(AttackType::NormalAttack);
	}


	AttackType AttackParameterTable::ConvertAttackType(const std::string& attackName)
	{
		/* 攻撃タイプの文字列をAttackType列挙型に変換する。 */
		if (attackName == "Normal" || attackName == "NormalAttack" || attackName == "AttackType::NormalAttack")
			return AttackType::NormalAttack;
		if (attackName == "Charge" || attackName == "ChargeAttack" || attackName == "AttackType::ChargeAttack")
			return AttackType::ChargeAttack;
		if (attackName == "Heavy" || attackName == "HeavyAttack" || attackName == "AttackType::HeavyAttack")
			return AttackType::HeavyAttack;
		if (attackName == "SlashUp" || attackName == "AttackType::SlashUp")
			return AttackType::SlashUp;
		if (attackName == "Air" || attackName == "AirAttack" || attackName == "AttackType::AirAttack")
			return AttackType::AirAttack;
		if (attackName == "RushStart" || attackName == "RushAttack_Start" || attackName == "AttackType::RushAttack_Start")
			return AttackType::RushAttack_Start;
		if (attackName == "RushEnd" || attackName == "RushAttack_End" || attackName == "AttackType::RushAttack_End")
			return AttackType::RushAttack_End;
		if (attackName == "Push" || attackName == "PushForward" || attackName == "AttackType::PushForward")
			return AttackType::PushForward;
		if (attackName == "Magic" || attackName == "MagicAttack" || attackName == "AttackType::MagicAttack")
			return AttackType::MagicAttack;
		if (attackName == "Heal" || attackName == "HeelMagic" || attackName == "AttackType::HeelMagic")
			return AttackType::HeelMagic;

		return AttackType::NormalAttack;
	}


	AttackParameter AttackParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex, AttackType attackType)
	{
		/* 攻撃タイプごとのデフォルト値を取得する。 */
		const AttackParameter& baseParameter = GetDefaultParameter(attackType);

		/* TSVの1行からAttackParameterを作成する。 */
		return AttackParameter
		{
			table.GetFloat(rowIndex, "DamageMultiplier", baseParameter.damageMultiplier),
			table.GetFloat(rowIndex, "CriticalRatel", table.GetFloat(rowIndex, "CriticalRate", baseParameter.criticalRatel)),
			table.GetFloat(rowIndex, "KnockBackPower", baseParameter.knockBackPower),
			table.GetFloat(rowIndex, "Range", baseParameter.range),
			table.GetFloat(rowIndex, "Radius", baseParameter.radius),
			table.GetInt(rowIndex, "HitStopFrame", baseParameter.hitStopFrame),
			table.GetInt(rowIndex, "CoolTimer", baseParameter.coolTimer),
			table.GetInt(rowIndex, "ComboNextTime", baseParameter.comboNextTime)
		};
	}
}
