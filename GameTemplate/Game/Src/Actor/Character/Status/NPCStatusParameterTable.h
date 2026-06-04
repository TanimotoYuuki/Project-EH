#pragma once
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include <unordered_map>
#include <string>

namespace nsApp
{
	class TSVTable;

	struct NPCStatusParameter
	{
		int hp = 1000;
		float damageRate = 0.4f;
		float moveRate = 1.0f;
		int attackInterval = 60;
		int helpPriority = 1;
	};


	class NPCStatusParameterTable
	{
	public:
		/**
		 * @brief TSVファイルからNPCのステータスパラメータを読み込む
		 * @param filePath TSVファイルのパス
		 * @return 読み込み成功ならtrue、失敗ならfalse
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 指定した武器タイプに対応するNPCのステータスパラメータを取得する
		 * @param weaponType 武器タイプ
		 * @return 対応するNPCのステータスパラメータへの参照
		 */
		static const NPCStatusParameter& GetParameter(WeaponType weaponType);


	private:
		/**
		 * @brief 武器名からWeaponTypeを変換する
		 * @param weaponName 武器名（例: "Sword", "Axe", "Spear"など）
		 * @return 対応するWeaponType。該当する武器がない場合はWeaponType::Noneを返す
		 */
		static WeaponType ConvertWeaponType(const std::string& weaponName);

		/**
		 * @brief TSVテーブルの行からNPCStatusParameterを作成する
		 * @param table TSVテーブル
		 * @param rowIndex 行インデックス
		 * @return 
		 */
		static NPCStatusParameter CreateParameterFromRow(const TSVTable& table, int rowIndex);


	private:
		static std::unordered_map<WeaponType, NPCStatusParameter> m_table; //! 武器タイプとNPCのステータスパラメータのマッピング
	};
}
