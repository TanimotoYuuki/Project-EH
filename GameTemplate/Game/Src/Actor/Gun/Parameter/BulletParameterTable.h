#pragma once
/**
 * @file   BulletParameterTable.h
 * @brief  各弾丸のパラメータを管理する。
 * @author Yamaguchi Hayato
 * @date   2026/05/05
 */

#include "Src/Actor/Gun/Register/BulletModelRegister.h"
#include "Src/Actor/Gun/Parameter/BulletParameter.h"
#include <unordered_map>


namespace nsApp
{
	class TSVTable;
	class BulletParameterTable
	{
	public:
		/**
		 * @brief TSVファイルから弾丸のパラメータを読み込む。
		 * @param filePath 読み込むTSVファイルのパス。
		 * @return 読み込みに成功した場合はtrue。
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 弾丸の種類に応じたパラメータを取得する。
		 * @param kindBullet 弾丸の種類。
		 * @return 弾丸のパラメータ。
		 */
		static const BulletParameter& GetParameter(BulletType kindBullet);

		/**
		 * @brief 弾丸のモデルファイルのパスを取得する。
		 * @param name モデル名。
		 * @return モデルファイルのパス。
		 */
		static inline std::string GetModelPath(const std::string& name)
		{
			return "Assets/modelData/Character/Weapon/" + name + ".tkm";
		}


	private:
		/**
		 * @brief TSVの文字列から弾丸の種類に変換する。
		 * @param typeName TSV上の弾丸名。
		 * @return 弾丸の種類。
		 */
		static BulletType ConvertBulletType(const std::string& typeName);

		/**
		 * @brief TSVの1行から弾丸パラメータを作成する。
		 * @param table TSVテーブル。
		 * @param rowIndex 行番号。
		 * @param bulletType 弾丸の種類。
		 * @return 弾丸パラメータ。
		 */
		static BulletParameter CreateParameterFromRow(const TSVTable& table, int rowIndex, BulletType bulletType);


	private:
		static std::unordered_map<BulletType, BulletParameter> m_bulletParameterTable;

	};
}
