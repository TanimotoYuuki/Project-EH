#pragma once

/**
 * @file   TSVTableLoder.h
 * @brief  TSVファイルからパラメーターテーブルを作成するヘルパークラス。
 * @author Yamaguchi Hayato
 * @date   2026/06/02
 */

#include "Src/Utilty/TSVTable.h"

#include <string>
#include <unordered_map>

namespace nsApp
{
	class TSVTableLoder
	{
	public:
		/**
		 * @brief TSVファイルからパラメーターテーブルを読み込む。
		 * @param filePath TSVファイルのパス。
		 * @param keyColumnName キーとして使用する列名。
		 * @param outputTable 読み込み結果を格納するテーブル。
		 * @param keyConverter 文字列をキー型へ変換する関数。
		 * @param rowCreator TSVの1行からパラメータを作成する関数。
		 * @return 読み込みに成功した場合はtrue。
		 */
		template<class KeyType, class ParameterType, class KeyConverter, class RowCreator>
		static bool LoadTable(
			const char* filePath,
			const char* keyColumnName,
			std::unordered_map<KeyType, ParameterType>& outputTable,
			KeyConverter keyConverter,
			RowCreator rowCreator
		);


		/**
		 * @brief TSVファイルからパラメーターテーブルを読み込む。
		 * @param filePath TSVファイルのパス。
		 * @param primaryKeyColumnName 優先して使用するキー列名。
		 * @param secondaryKeyColumnName 予備として使用するキー列名。
		 * @param outputTable 読み込み結果を格納するテーブル。
		 * @param keyConverter 文字列をキー型へ変換する関数。
		 * @param rowCreator TSVの1行からパラメータを作成する関数。
		 * @return 読み込みに成功した場合はtrue。
		 */
		template<class KeyType, class ParameterType, class KeyConverter, class RowCreator>
		static bool LoadTable(
			const char* filePath,
			const char* primaryKeyColumnName,
			const char* secondaryKeyColumnName,
			std::unordered_map<KeyType, ParameterType>& outputTable,
			KeyConverter keyConverter,
			RowCreator rowCreator
		);
	};
}

#include "TSVTableLoder.inl"