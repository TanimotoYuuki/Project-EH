#include "stdafx.h"
#include "NPCActionParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
	/* テーブルを初期化。*/
	std::unordered_map<NPCActionType, NPCActionParameter> NPCActionParameterTable::m_table;


	bool NPCActionParameterTable::LoadTSVFile(const char* filePath)
	{
		// TSVファイルを読み込む。
		TSVTable table;
		if (!table.LoadTSVFile(filePath))
			return false;

		/* 読み込んだ情報をテーブルに置き換える。*/
		std::unordered_map<NPCActionType, NPCActionParameter> loadedTable;
		for (int rowIndex = 0; rowIndex < table.GetRowCount(); ++rowIndex)
		{
			// 行からアクションタイプを取得する。
			const std::string actionName = table.GetString(rowIndex, "NPCActionType");
			if (actionName.empty())
				continue;

			// アクションタイプを変換して、テーブルにパラメータを追加する。
			loadedTable[ConvertActionType(actionName)] = CreateParameterFromRow(table, rowIndex);
		}

		/* 空データ出ないことを確認する。*/
		if (loadedTable.empty())
			return false;

		/* 情報を入れ替える。*/
		m_table.swap(loadedTable);
		return true;
	}


	const NPCActionParameter& NPCActionParameterTable::GetParameter(NPCActionType actionType)
	{
		/* テーブルを探索する。*/
		auto iterator = m_table.find(actionType);
		if (iterator != m_table.end())
			return iterator->second;

		/* 見つからない場合は、デフォルトのパラメータを返す。*/
		static const NPCActionParameter fallbackParameter;
		return fallbackParameter;
	}

	NPCActionType NPCActionParameterTable::ConvertActionType(const std::string& actionName)
	{
		/* アクション名をアクションタイプに変換する。*/
		if (actionName == "Help")
			return NPCActionType::Help;
		if (actionName == "Chase")
			return NPCActionType::Chase;
		if (actionName == "Attack")
			return NPCActionType::Attack;
		if (actionName == "Idle")
			return NPCActionType::Idle;

		/* 見つからない場合は、Noneを返す。*/
		return NPCActionType::None;
	}


	NPCActionParameter NPCActionParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex)
	{
		/* 行からパラメータを作成する。*/
		return NPCActionParameter
		{
			table.GetFloat(rowIndex, "MoveSpeed", 1.0f),
			table.GetFloat(rowIndex, "DashSpeed", 1.0f),
			table.GetFloat(rowIndex, "SearchRange", 900.0f),
			table.GetFloat(rowIndex, "AttackRange", 150.0f),
			table.GetFloat(rowIndex, "HelpRange", 3.0f),
			table.GetFloat(rowIndex, "CoolTime", 0.0f)
		};
	}
}
