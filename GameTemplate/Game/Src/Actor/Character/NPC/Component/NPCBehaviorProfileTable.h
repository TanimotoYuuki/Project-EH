#pragma once

#include "NPCBehaviorProfile.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include <string>
#include <unordered_map>

namespace nsApp
{
	class TSVTable;

	class NPCBehaviorProfileTable
	{
	public:
		/**
		 * @brief TSVファイルを読み込み、NPCBehaviorProfileのテーブルを作成する。
		 * @param filePath 読み込むTSVファイルのパス。
		 * @return 読み込みに成功した場合はtrue、失敗した場合はfalse。
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 指定されたWeaponTypeに対応するNPCBehaviorProfileを取得する。
		 * @param weaponType 取得するWeaponType。
		 * @return 指定されたWeaponTypeに対応するNPCBehaviorProfileの参照。
		 */
		static const NPCBehaviorProfile& GetParameter(WeaponType weaponType);


	private:
		/**
		 * @brief WeaponTypeを文字列から変換する。
		 * @param weaponName 変換する文字列。
		 * @return 変換されたWeaponType。
		 */
		static WeaponType ConvertWeaponType(const std::string& weaponName);

		/**
		 * @brief TSVの行からNPCBehaviorProfileを作成する。
		 * @param table TSVTableの参照。
		 * @param rowIndex 作成する行のインデックス。
		 * @return 作成されたNPCBehaviorProfile。
		 */
		static NPCBehaviorProfile CreateParameterFromRow(const TSVTable& table, int rowIndex);


	private:
		static std::unordered_map<WeaponType, NPCBehaviorProfile> m_table; //! WeaponTypeをキーとしたNPCBehaviorProfileのテーブル。
	};
}