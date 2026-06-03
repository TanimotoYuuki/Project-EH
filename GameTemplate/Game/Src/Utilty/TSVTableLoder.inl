#pragma once

namespace nsApp
{
	template<class KeyType, class ParameterType, class KeyConverter, class RowCreator>
	bool TSVTableLoder::LoadTable(
		const char* filePath,
		const char* keyColumnName,
		std::unordered_map<KeyType, ParameterType>& outputTable,
		KeyConverter keyConverter,
		RowCreator rowCreator
	)
	{
		/* キー列が1種類だけの場合は、予備キー列なしの読み込み処理へ渡す。 */
		return LoadTable(
			filePath,
			keyColumnName,
			nullptr,
			outputTable,
			keyConverter,
			rowCreator
		);
	}


	template<class KeyType, class ParameterType, class KeyConverter, class RowCreator>
	bool TSVTableLoder::LoadTable(
		const char* filePath,
		const char* primaryKeyColumnName,
		const char* secondaryKeyColumnName,
		std::unordered_map<KeyType, ParameterType>& outputTable,
		KeyConverter keyConverter,
		RowCreator rowCreator
	)
	{
		/* TSVファイルを読み込むための一時テーブルを作成する。 */
		TSVTable table;

		/* TSVファイルを開けなかった場合は読み込み失敗にする。 */
		if (!table.LoadTSVFile(filePath))
			return false;

		/* 読み込みに失敗しても既存テーブルを壊さないように、一時テーブルへ格納する。 */
		std::unordered_map<KeyType, ParameterType> loadedTable;

		/* TSVの全データ行を確認する。 */
		for (int rowIndex = 0; rowIndex < table.GetRowCount(); ++rowIndex)
		{
			/* まずはメインのキー列からキー名を取得する。 */
			std::string keyName = table.GetString(rowIndex, primaryKeyColumnName);

			/* メインのキー列が空で、予備キー列が指定されている場合は予備キー列を見る。 */
			if (keyName.empty() && secondaryKeyColumnName != nullptr)
				keyName = table.GetString(rowIndex, secondaryKeyColumnName);

			/* キー名が取れない行は無効行として無視する。 */
			if (keyName.empty())
				continue;

			/* 文字列のキー名をenumなどのキー型へ変換する。 */
			const KeyType key = keyConverter(keyName);

			/* 1行分のTSVデータからパラメータ構造体を作成して登録する。 */
			loadedTable[key] = rowCreator(table, rowIndex, key);
		}

		/* 有効なデータが1件も読み込めなかった場合は失敗にする。 */
		if (loadedTable.empty())
			return false;

		/* 読み込みに成功した場合だけ、正式なテーブルへ反映する。 */
		outputTable.swap(loadedTable);

		return true;
	}
}