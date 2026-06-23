#pragma once
#include "Src/Actor/Character/Player/Component/Guard/GuardParameter.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/Utilty/TSVTable.h"

#include <string>
#include <unordered_map>


namespace nsApp
{
	class GuardParameterTable
	{
	public:
		/* コンストラクタとデストラクタ。*/
		GuardParameterTable() = default;
		virtual ~GuardParameterTable() = default;

		/**
		 * @brief TSVファイルを読み込む関数。指定されたファイルパスのTSVファイルを読み込み、ガードパラメータを設定する。
		 * @param filePath 読み込むTSVファイルのパス。
		 * @return TSVファイルの読み込みに成功した場合は true、失敗した場合は false を返す。
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 指定された武器タイプに対応するガードパラメータを取得する関数。
		 * @param weponType 取得するガードパラメータに対応する武器タイプ。
		 * @return 指定された武器タイプに対応するガードパラメータの参照。
		 */
		static const GuardParameter& GetParameter(WeaponType weponType);


	private:
		/**
		 * @brief 武器名を武器タイプに変換する関数。
		 * @param weaponName 変換する武器名。
		 * @return 変換された武器タイプ。
		 */
		static WeaponType ConvertWeaponType(const std::string& weaponName);

		/**
		 * @brief TSVテーブルの指定された行からガードパラメータを作成する関数。
		 * @param table TSVテーブルの参照。
		 * @param rowIndex ガードパラメータを作成する行のインデックス。
		 * @return 作成されたガードパラメータ。
		 */
		static GuardParameter CreateParameterFromRow(const TSVTable& table, int rowIndex);


	private:
		static std::unordered_map<WeaponType, GuardParameter> m_table; //! 武器タイプとガードパラメータのマッピングを保持する静的メンバ変数。
	};
}