#include "stdafx.h"
#include "PlayerStatusParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
	/* テーブルを初期化。*/
	std::unordered_map<WeaponType, PlayerStatusParameter> PlayerStatusParameterTable::m_table;


	bool PlayerStatusParameterTable::LoadTSVFile(const char* filePath)
	{
		/* TSVファイルを読み込む。*/
		TSVTable table;
		/* ファイルの読み込みに失敗した場合はfalseを返す。*/
		if (!table.LoadTSVFile(filePath))
			return false;

		/* 読み込んだテーブルからパラメータを作成し、テーブルに格納する。*/
		std::unordered_map<WeaponType, PlayerStatusParameter> loadedTable;
		for (int rowIndex = 0; rowIndex < table.GetRowCount(); ++rowIndex)
		{
			/* 武器タイプを取得する。*/
			const std::string weaponName = table.GetString(rowIndex, "WeaponType");
			/* 武器タイプが空の場合はスキップする。*/
			if (weaponName.empty())
				continue;

			/* 武器タイプを変換し、パラメータを作成してテーブルに格納する。*/
			loadedTable[ConvertWeaponType(weaponName)] = CreateParameterFromRow(table, rowIndex);
		}

		/* 読み込んだテーブルが空の場合はfalseを返す。*/
		if (loadedTable.empty())
			return false;

		/* 読み込んだテーブルをメンバーテーブルにスワップする。*/
		m_table.swap(loadedTable);
		return true;
	}


	const PlayerStatusParameter& PlayerStatusParameterTable::GetParameter(WeaponType weaponType)
	{
		/* 指定された武器タイプに対応するパラメータをテーブルから取得する。*/
		auto iterator = m_table.find(weaponType);
		if (iterator != m_table.end())
			return iterator->second;

		/* 対応するパラメータが見つからない場合は、デフォルトのパラメータを返す。*/
		static const PlayerStatusParameter fallbackParameter;
		return fallbackParameter;
	}


	WeaponType PlayerStatusParameterTable::ConvertWeaponType(const std::string& weaponName)
	{
		/* 武器名を武器タイプに変換する。*/
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


	PlayerStatusParameter PlayerStatusParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex)
	{
		/* テーブルの行からパラメータを作成する。*/
		return PlayerStatusParameter
		{
			table.GetInt(rowIndex, "MaxHP", 1000),
			table.GetFloat(rowIndex, "NormalDamage", 40.0f),
			table.GetFloat(rowIndex, "CriticalDamage", 1.5f),
			table.GetFloat(rowIndex, "WalkSpeed", 1.0f),
			table.GetFloat(rowIndex, "RunSpeed", 1.5f),
			table.GetFloat(rowIndex, "JumpPower", 500.0f),
			table.GetFloat(rowIndex, "AirMoveSpeed", 120.0f),
			table.GetFloat(rowIndex, "Gravity", 30.0f),
			table.GetFloat(rowIndex, "MaxFallVelocity", -1200.0f)
		};
	}
}
