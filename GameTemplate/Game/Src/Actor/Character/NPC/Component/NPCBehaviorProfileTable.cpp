#include "stdafx.h"
#include "NPCBehaviorProfileTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
	std::unordered_map<WeaponType, NPCBehaviorProfile> NPCBehaviorProfileTable::m_table;


	bool NPCBehaviorProfileTable::LoadTSVFile(const char* filePath)
	{
		/* TSVファイルを読み込む。*/
		TSVTable table;
		if (!table.LoadTSVFile(filePath))
			return false;

		/* 読み込んだTSVの内容をNPCBehaviorProfileのテーブルに変換する。*/
		std::unordered_map<WeaponType, NPCBehaviorProfile> loadedTable;
		for (int rowIndex = 0; rowIndex < table.GetRowCount(); ++rowIndex)
		{
			/* WeaponType列の値を取得する。*/
			const std::string weaponName = table.GetString(rowIndex, "WeaponType");
			if (weaponName.empty())
				continue;

			/* WeaponType列の値をWeaponTypeに変換する。*/
			const WeaponType weaponType = ConvertWeaponType(weaponName);
			if (weaponType == WeaponType::None)
				continue;

			/* NPCBehaviorProfileを作成し、テーブルに追加する。*/
			loadedTable[weaponType] = CreateParameterFromRow(table, rowIndex);
		}

		/* 読み込んだテーブルが空でない場合、m_tableに反映する。*/
		if (loadedTable.empty())
			return false;

		/* 読み込んだテーブルをm_tableに反映する。*/
		m_table.swap(loadedTable);
		return true;
	}


	const NPCBehaviorProfile& NPCBehaviorProfileTable::GetParameter(WeaponType weaponType)
	{
		/* m_tableから指定されたWeaponTypeに対応するNPCBehaviorProfileを取得する。*/
		auto iterator = m_table.find(weaponType);
		if (iterator != m_table.end())
			return iterator->second;

		/* 指定されたWeaponTypeに対応するNPCBehaviorProfileが存在しない場合、デフォルトのNPCBehaviorProfileを返す。*/
		static const NPCBehaviorProfile fallbackParameter;
		return fallbackParameter;
	}


	WeaponType NPCBehaviorProfileTable::ConvertWeaponType(const std::string& weaponName)
	{
		/* weaponNameの値をWeaponTypeに変換する。複数の表記に対応する。*/
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


	NPCBehaviorProfile NPCBehaviorProfileTable::CreateParameterFromRow(const TSVTable& table, int rowIndex)
	{
		/* 回避行動の開始距離と終了距離を取得する。終了距離が未設定の場合、開始距離の65%を使用する。*/
		const float enterThreshold = table.GetFloat(rowIndex, "EvadeThreshold", 0.0f);
		const float exitThreshold = table.GetFloat(rowIndex, "EvadeExitThreshold", enterThreshold * 0.65f);

		return NPCBehaviorProfile
		{
			table.GetFloat(rowIndex, "PreferredAttackRange", 170.0f),
			enterThreshold,
			exitThreshold,
			table.GetInt(rowIndex, "EvadeReactionDelayMin", 0),
			table.GetInt(rowIndex, "EvadeReactionDelayMax", 0),
			table.GetFloat(rowIndex, "ThreatMarginScale", 1.0f),
			table.GetFloat(rowIndex, "Aggression", 1.0f),
			table.GetFloat(rowIndex, "RangedChaseEnterOffset", 30.0f),
			table.GetFloat(rowIndex, "RangedChaseExitOffset", 15.0f),
			table.GetFloat(rowIndex, "RangedRetreatEnterOffset", 50.0f),
			table.GetFloat(rowIndex, "RangedRetreatExitOffset", 35.0f),
			table.GetInt(rowIndex, "PreferRunInEvade", 1) != 0
		};
	}
}