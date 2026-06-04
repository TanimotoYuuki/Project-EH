#include "stdafx.h"
#include "NPCStatusParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
	/* テーブルを初期化。*/
	std::unordered_map<WeaponType, NPCStatusParameter> NPCStatusParameterTable::m_table;


	bool NPCStatusParameterTable::LoadTSVFile(const char* filePath)
	{
		/* TSVファイルを読み込む。*/
		TSVTable table;
		/* ファイルの読み込みに失敗した場合はfalseを返す。*/
		if (!table.LoadTSVFile(filePath))
			return false;

		/* 読み込んだテーブルからNPCのステータスパラメータを作成する。*/
		std::unordered_map<WeaponType, NPCStatusParameter> loadedTable;
		for (int rowIndex = 0; rowIndex < table.GetRowCount(); ++rowIndex)
		{
			/* 武器タイプを取得する。*/
			const std::string weaponName = table.GetString(rowIndex, "WeaponType");
			if (weaponName.empty())
				continue;

			/* 武器タイプを変換して、NPCのステータスパラメータを作成する。*/
			loadedTable[ConvertWeaponType(weaponName)] = CreateParameterFromRow(table, rowIndex);
		}

		/* 読み込んだテーブルが空の場合はfalseを返す。*/
		if (loadedTable.empty())
			return false;

		/* 読み込んだテーブルをメンバ変数に設定する。*/
		m_table.swap(loadedTable);
		return true;
	}


	const NPCStatusParameter& NPCStatusParameterTable::GetParameter(WeaponType weaponType)
	{
		/* 指定された武器タイプに対応するNPCのステータスパラメータを返す。*/
		auto iterator = m_table.find(weaponType);
		if (iterator != m_table.end())
			return iterator->second;

		/* 対応する武器タイプが見つからない場合は、デフォルトのNPCのステータスパラメータを返す。*/
		static const NPCStatusParameter fallbackParameter;
		return fallbackParameter;
	}


	WeaponType NPCStatusParameterTable::ConvertWeaponType(const std::string& weaponName)
	{
		/* 武器タイプの文字列を対応するWeaponType列挙型に変換する。*/
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


	NPCStatusParameter NPCStatusParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex)
	{
		/* 指定された行からNPCのステータスパラメータを作成する。*/
		return NPCStatusParameter
		{
			table.GetInt(rowIndex, "HP", 1000),
			table.GetFloat(rowIndex, "DamageRate", 0.4f),
			table.GetFloat(rowIndex, "MoveRate", 1.0f),
			table.GetInt(rowIndex, "AttackInterval", 60),
			table.GetInt(rowIndex, "HelpPriority", 1)
		};
	}
}
