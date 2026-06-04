#include "stdafx.h"
#include "TSVTable.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

namespace nsApp
{
	bool TSVTable::LoadTSVFile(const char* filePath)
	{
		/* 読み込み済みデータを初期化する。 */
		Clear();

		/* 読み込むファイルパスを保持する。 */
		m_filePath = filePath != nullptr ? filePath : "";

		/* ファイルパスが空なら読み込みを中断する。 */
		if (m_filePath.empty())
		{
			m_lastError = "TSVファイルのパスが空です。";
			return false;
		}

		/* TSVファイルを開く。 */
		std::ifstream file(m_filePath.c_str());

		/* TSVファイルを開けなかった場合はエラーにする。 */
		if (!file.is_open())
		{
			m_lastError = "TSVファイルを開けませんでした: " + m_filePath;
			return false;
		}

		/* ヘッダー行を読み込む。 */
		if (!std::getline(file, m_lineBuffer))
		{
			m_lastError = "TSVファイルが空です: " + m_filePath;
			return false;
		}

		/* UTF-8 BOMが付いている場合は取り除く。 */
		m_lineBuffer = RemoveUTF8BOM(m_lineBuffer);

		/* ヘッダー行を列名として分割する。 */
		m_header = SplitLine(m_lineBuffer);

		/* 列名から列番号を検索できるように登録する。 */
		for (int i = 0; i < static_cast<int>(m_header.size()); ++i)
		{
			m_header[i] = Trim(m_header[i]);

			if (!m_header[i].empty())
				m_columnIndexTable[m_header[i]] = i;
		}

		/* データ行を最後まで読み込む。 */
		while (std::getline(file, m_lineBuffer))
		{
			/* 行の前後の空白を取り除く。 */
			m_lineBuffer = Trim(m_lineBuffer);

			/* 空行は無視する。 */
			if (m_lineBuffer.empty())
				continue;

			/* 行を列ごとに分割して保存する。 */
			m_rows.push_back(SplitLine(m_lineBuffer));
		}

		return true;
	}


	std::string TSVTable::GetString(int rowIndex, const char* columnName, const char* defaultValue) const
	{
		/* 行番号が不正な場合はデフォルト値を返す。 */
		if (rowIndex < 0 || rowIndex >= static_cast<int>(m_rows.size()))
			return defaultValue;

		/* 列名が存在しない場合はデフォルト値を返す。 */
		const auto columnIterator = m_columnIndexTable.find(columnName);

		/* 列名が存在しない場合はデフォルト値を返す。 */
		if (columnIterator == m_columnIndexTable.end())
			return defaultValue;

		/* 列番号が不正な場合はデフォルト値を返す。 */
		const int columnIndex = columnIterator->second;

		/* 列番号が不正な場合はデフォルト値を返す。 */
		if (columnIndex < 0 || columnIndex >= static_cast<int>(m_rows[rowIndex].size()))
			return defaultValue;

		/* セルの値を返す。 */
		return Trim(m_rows[rowIndex][columnIndex]);
	}


	float TSVTable::GetFloat(int rowIndex, const char* columnName, float defaultValue) const
	{
		/* セルの値を文字列として取得する。 */
		const std::string value = GetString(rowIndex, columnName, "");

		/* 文字列が空の場合はデフォルト値を返す。 */
		if (value.empty())
			return defaultValue;

		/* 文字列をfloatに変換して返す。 */
		return static_cast<float>(std::atof(value.c_str()));
	}


	int TSVTable::GetInt(int rowIndex, const char* columnName, int defaultValue) const
	{
		/* セルの値を文字列として取得する。 */
		const std::string value = GetString(rowIndex, columnName, "");

		/* 文字列が空の場合はデフォルト値を返す。 */
		if (value.empty())
			return defaultValue;

		/* 文字列をintに変換して返す。 */
		return std::atoi(value.c_str());
	}


	void TSVTable::Clear()
	{
		/* 読み込み元パスを初期化する。 */
		m_filePath.clear();

		/* 行バッファを初期化する。 */
		m_lineBuffer.clear();

		/* ヘッダー情報を初期化する。 */
		m_header.clear();

		/* データ行を初期化する。 */
		m_rows.clear();

		/* 列名検索テーブルを初期化する。 */
		m_columnIndexTable.clear();

		/* エラー内容を初期化する。 */
		m_lastError.clear();
	}


	std::vector<std::string> TSVTable::SplitLine(const std::string& line)
	{
		/* タブ区切りで行を分割する。 */
		std::vector<std::string> result;
		std::string cell;
		std::stringstream stream(line);

		/* タブ区切りで行を分割する。 */
		while (std::getline(stream, cell, '\t'))
			result.push_back(Trim(cell));

		return result;
	}


	std::string TSVTable::Trim(const std::string& text)
	{
		/* 文字列の前後の空白を取り除く。 */
		const char* whiteSpace = " \t\r\n";
		const size_t begin = text.find_first_not_of(whiteSpace);

		/* 文字列が空白だけで構成されている場合は空文字を返す。 */
		if (begin == std::string::npos)
			return "";

		/* 文字列の前後の空白を取り除く。 */
		const size_t end = text.find_last_not_of(whiteSpace);

		/* 文字列の前後の空白を取り除いた文字列を返す。 */
		return text.substr(begin, end - begin + 1);
	}


	std::string TSVTable::RemoveUTF8BOM(const std::string& text)
	{
		/* UTF-8のBOMは3バイトで構成されているため、文字列の長さが3未満の場合はBOMが存在しないと判断する。 */
		if (text.size() < 3)
			return text;

		/* 文字列の先頭3バイトを取得する。 */
		const unsigned char bom0 = static_cast<unsigned char>(text[0]);
		const unsigned char bom1 = static_cast<unsigned char>(text[1]);
		const unsigned char bom2 = static_cast<unsigned char>(text[2]);

		/* UTF-8のBOMは0xEF、0xBB、0xBFの3バイトで構成されているため、先頭3バイトがBOMと一致する場合はBOMを取り除いた文字列を返す。 */
		if (bom0 == 0xEF && bom1 == 0xBB && bom2 == 0xBF)
			return text.substr(3);

		return text;
	}
}
