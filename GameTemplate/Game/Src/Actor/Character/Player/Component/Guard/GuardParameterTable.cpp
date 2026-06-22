#include "stdafx.h"
#include "GuardParameterTable.h"
#include "Src/Utilty/TSVTableLoder.h"


namespace nsApp
{
	/* 静的メンバ変数の定義。*/
	std::unordered_map<WeaponType, GuardParameter> GuardParameterTable::m_table;


	bool GuardParameterTable::LoadTSVFile(const char* filePath)
	{
		return TSVTableLoder::LoadTable<WeaponType, GuardParameter>(filePath,
			"WeaponType",
			"WeponType",
			m_table,
			GuardParameterTable::ConvertWeaponType, 
			[](const TSVTable& table, int rowIndex, WeaponType) {
				return GuardParameterTable::CreateParameterFromRow(table, rowIndex);
			}
		);
	}


	const GuardParameter& GuardParameterTable::GetParameter(WeaponType weaponType)
	{
		/* 指定された武器タイプに対応するガードパラメータを取得する。存在しない場合はデフォルトのガードパラメータを返す。*/
		auto iterator = m_table.find(weaponType);

		/* 指定された武器タイプに対応するガードパラメータが存在する場合はそれを返す。*/
		if (iterator != m_table.end())
			return iterator->second;

		/* 指定された武器タイプに対応するガードパラメータが存在しない場合はデフォルトのガードパラメータを返す。*/
		static const GuardParameter fallbackParameter;
		return fallbackParameter;
	}


	WeaponType GuardParameterTable::ConvertWeaponType(const std::string& weaponName)
	{
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


	GuardParameter GuardParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex)
	{
		/* TSVテーブルの指定された行からガードパラメータを作成する。*/
		float reduction = table.GetFloat(rowIndex, "DamageReduction", 0.5f);
		if (reduction < 0.0f) 
			reduction = 0.0f;
		if (reduction > 1.0f) 
			reduction = 1.0f;

		/* ガードパラメータを作成して返す。*/
		return GuardParameter
		{
			table.GetFloat(rowIndex, "MaxDuration", 2.0f),
			reduction,
			table.GetInt(rowIndex, "BlockKnockback", 1) != 0,
		};
	}
}