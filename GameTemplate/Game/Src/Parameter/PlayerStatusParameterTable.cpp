#include "stdafx.h"
#include "PlayerStatusParameterTable.h"

#include "Src/Utilty/TSVTableLoder.h"


namespace
{
	const auto ROW_INDEX_NUM = 0;
}

namespace nsApp
{
	std::unordered_map<WeaponType, PlayerStatusParameter> PlayerStatusParameterTable::m_table;


	bool PlayerStatusParameterTable::LoadTSVFile(const char* filePath)
	{
		return TSVTableLoder::LoadTable<WeaponType, PlayerStatusParameter>
			(
				/* TSVファイルのパス。*/
				filePath,
				/* キー列名。*/
				"WeaponType",
				/* 読み込んだデータを格納するテーブル。*/
				m_table,
				/* キー列の文字列をWeaponTypeに変換する関数。*/
				PlayerStatusParameterTable::ConvertWeaponType,
				[](const TSVTable& table, int rowIndex, WeaponType)
				{
					/* TSVの1行からPlayerStatusParameterを作成する。 */
					return PlayerStatusParameterTable::CreateParameterFromRow(table, rowIndex);
				}
			);
	}


	const PlayerStatusParameter& PlayerStatusParameterTable::GetParameter(WeaponType weaponType)
	{
		/* WeaponTypeに対応するPlayerStatusParameterをテーブルから取得する。*/
		auto iterator = m_table.find(weaponType);

		/* WeaponTypeに対応するPlayerStatusParameterが存在する場合、PlayerStatusParameterを返す。*/
		if (iterator != m_table.end())
			return iterator->second;

		/* WeaponTypeに対応するPlayerStatusParameterが存在しない場合、デフォルトのPlayerStatusParameterを返す。*/
		static const PlayerStatusParameter fallBackParameter =
		{
			1000,     //! maxHP。
			20.0f,    //! normalDamage。
			1.5f,     //! criticalDamage。
			0.05f,    //! criticalRate。

			1.0f,     //! walkSpeed。
			1.5f,     //! runSpeed。
			500.0f,   //! jumpPower。
			120.0f,   //! airMoveSpeed。
			30.0f,    //! gravity。
			-1200.0f  //! maxFallVelocity。
		};

		return fallBackParameter;
	}


	WeaponType PlayerStatusParameterTable::ConvertWeaponType(const std::string& weaponName)
	{
		/* TSVの文字列をWeaponTypeに変換する。*/
		if (weaponName == "Sword")
			return WeaponType::GreatSword;

		if (weaponName == "Hammer")
			return WeaponType::Hammer;

		if (weaponName == "Wand")
			return WeaponType::Wand;

		if (weaponName == "TwinGun")
			return WeaponType::TwinGun;

		return WeaponType::None;
	}


	PlayerStatusParameter PlayerStatusParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex)
	{
		/* TSVの行からPlayerStatusParameterを作成する。*/
		return PlayerStatusParameter
		{
			table.GetInt(rowIndex, "MaxHP", 1000),
			table.GetFloat(rowIndex, "NormalDamage", 20.0f),
			table.GetFloat(rowIndex, "CriticalDamage", 1.5f),
			table.GetFloat(rowIndex, "CriticalRate", 0.05f),

			table.GetFloat(rowIndex, "WalkSpeed", 1.0f),
			table.GetFloat(rowIndex, "RunSpeed", 1.5f),
			table.GetFloat(rowIndex, "JumpPower", 500.0f),
			table.GetFloat(rowIndex, "AirMoveSpeed", 120.0f),
			table.GetFloat(rowIndex, "Gravity", 30.0f),
			table.GetFloat(rowIndex, "MaxFallVelocity", -1200.0f)
		};
	}
}