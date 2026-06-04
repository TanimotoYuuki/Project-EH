#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @file   TSVTable.h
 * @brief  TSVファイルを読み込む汎用テーブルクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/29
 */

namespace nsApp
{
	class TSVTable
	{
	public:
		/**
		 * @brief TSVファイルを読み込む。
		 * @param filePath 読み込むTSVファイルのパス。
		 * @return 読み込みに成功した場合はtrue。
		 */
		bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 指定した行と列名から文字列を取得する。
		 * @param rowIndex 行番号。
		 * @param columnName 列名。
		 * @param defaultValue 取得できなかった場合の値。
		 * @return 取得した文字列。
		 */
		std::string GetString(int rowIndex, const char* columnName, const char* defaultValue = "") const;

		/**
		 * @brief 指定した行と列名からfloat値を取得する。
		 * @param rowIndex 行番号。
		 * @param columnName 列名。
		 * @param defaultValue 取得できなかった場合の値。
		 * @return 取得したfloat値。
		 */
		float GetFloat(int rowIndex, const char* columnName, float defaultValue = 0.0f) const;

		/**
		 * @brief 指定した行と列名からint値を取得する。
		 * @param rowIndex 行番号。
		 * @param columnName 列名。
		 * @param defaultValue 取得できなかった場合の値。
		 * @return 取得したint値。
		 */
		int GetInt(int rowIndex, const char* columnName, int defaultValue = 0) const;

		/**
		 * @brief 読み込んだデータ行数を取得する。
		 * @return データ行数。
		 */
		inline int GetRowCount() const
		{
			return static_cast<int>(m_rows.size());
		}

		/**
		 * @brief 指定した列が存在するか確認する。
		 * @param columnName 列名。
		 * @return 存在する場合はtrue。
		 */
		inline bool HasColumn(const char* columnName) const
		{
			return m_columnIndexTable.find(columnName) != m_columnIndexTable.end();
		}

		/**
		 * @brief 最後に発生したエラー内容を取得する。
		 * @return エラー文字列。
		 */
		inline const std::string& GetLastError() const
		{
			return m_lastError;
		}


	private:
		/**
		 * @brief 読み込み済みデータを初期化する。
		 */
		void Clear();

		/**
		 * @brief 1行をタブ区切りで分割する。
		 * @param line 対象の文字列。
		 * @return 分割後の文字列配列。
		 */
		static std::vector<std::string> SplitLine(const std::string& line);

		/**
		 * @brief 文字列の前後の空白を取り除く。
		 * @param text 対象の文字列。
		 * @return 空白を取り除いた文字列。
		 */
		static std::string Trim(const std::string& text);

		/**
		 * @brief UTF-8のBOMを取り除く。
		 * @param text 対象の文字列。
		 * @return BOMを取り除いた文字列。
		 */
		static std::string RemoveUTF8BOM(const std::string& text);


	private:
		std::string m_filePath;                                  //! 読み込んだTSVファイルのパス。
		std::string m_lineBuffer;                                //! 読み込み中の行バッファ。
		std::vector<std::string> m_header;                       //! 列名のリスト。
		std::vector<std::vector<std::string>> m_rows;            //! データ行のリスト。
		std::unordered_map<std::string, int> m_columnIndexTable; //! 列名から列番号への対応表。
		std::string m_lastError;                                 //! 最後に発生したエラー内容。
	};
}
