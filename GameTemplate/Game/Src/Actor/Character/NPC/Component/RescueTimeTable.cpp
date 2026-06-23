#include "stdafx.h"
#include "RescueTimeTable.h"
#include "Src/Utilty/TSVTableLoder.h"

namespace nsApp
{
	/* テーブルを初期化する。*/
	std::unordered_map<WeaponType, RescueTimeParameter> RescueTimeTable::m_table;

	
	bool RescueTimeTable::LoadTSVFile(const char* filePath)
	{
		/* WeaponType列を優先し、空の場合はWeponType列をキーとして使う。 */
		return TSVTableLoder::LoadTable<WeaponType, RescueTimeParameter>(
			filePath,
			"WeaponType",
			"WeponType",
			m_table,
			RescueTimeTable::ConvertWeaponType,
			[](const TSVTable& table, int rowIndex, WeaponType)
			{
				/* TSVの1行からRescueTimeParameterを作成する。 */
				return RescueTimeTable::CreateParameterFromRow(table, rowIndex);
			}
		);
	}


	const RescueTimeParameter& RescueTimeTable::GetParameter(WeaponType weaponType)
	{
		/* 武器タイプに対応するパラメータを検索する。*/
		auto iterator = m_table.find(weaponType);
		if (iterator != m_table.end())
			return iterator->second;

		/* 対応するパラメータが見つからない場合は、デフォルトのパラメータを返す。*/
		static const RescueTimeParameter fallbackParameter;
		return fallbackParameter;
	}


	WeaponType RescueTimeTable::ConvertWeaponType(const std::string& weaponName)
	{
		/* 武器タイプの列名から、WeaponType列挙型に変換する。*/
		if (weaponName == "Sword" || weaponName == "GreatSword" || weaponName == "WeaponType::GreatSword")
			return WeaponType::GreatSword;
		if (weaponName == "Hammer" || weaponName == "WeaponType::Hammer")
			return WeaponType::Hammer;
		if (weaponName == "Wand" || weaponName == "WeaponType::Wand")
			return WeaponType::Wand;
		if (weaponName == "TwinGun" || weaponName == "Gun" || weaponName == "WeaponType::TwinGun")
			return WeaponType::TwinGun;

		return WeaponType::None;
	}


	RescueTimeParameter RescueTimeTable::CreateParameterFromRow(const TSVTable& table, int rowIndex)
	{
		/* TSVの1行からRescueTimeParameterを作成する。必要な列がない場合は、デフォルト値を使用する。*/
		return RescueTimeParameter
		{
			table.GetInt(rowIndex, "RequiredHelpTime", 20),
			table.GetFloat(rowIndex, "RescueGaugeSpeed", 1.0f),
			table.GetInt(rowIndex, "HelpSEInterval", 30)
		};
	}
}
