#pragma once
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include <unordered_map>
#include <string>

namespace nsApp
{
	class TSVTable;

	struct PlayerStatusParameter
	{
		int maxHP = 1000;
		float normalDamage = 40.0f;
		float criticalDamage = 1.5f;
		float walkSpeed = 1.0f;
		float runSpeed = 1.5f;
		float jumpPower = 500.0f;
		float airMoveSpeed = 120.0f;
		float gravity = 30.0f;
		float maxFallVelocity = -1200.0f;
	};

	class PlayerStatusParameterTable
	{
	public:
		/**
		 * @brief TSVファイルからプレイヤーステータスパラメータを読み込む
		 * @param filePath TSVファイルのパス
		 * @return 読み込み成功ならtrue、失敗ならfalse
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 武器タイプに対応するプレイヤーステータスパラメータを取得する
		 * @param weaponType 武器タイプ
		 * @return 対応するプレイヤーステータスパラメータの参照
		 * @throws std::out_of_range 指定された武器タイプがテーブルに存在しない場合
		 */
		static const PlayerStatusParameter& GetParameter(WeaponType weaponType);


	private:
		/**
		 * @brief 武器名からWeaponTypeを変換する
		 * @param weaponName 武器名（例: "Sword", "Axe", "Bow"など）
		 * @return 対応するWeaponType
		 */
		static WeaponType ConvertWeaponType(const std::string& weaponName);

		/**
		 * @brief TSVテーブルの行からPlayerStatusParameterを作成する
		 * @param table TSVテーブルの参照
		 * @param rowIndex 行インデックス
		 * @return 作成されたPlayerStatusParameter
		 * @throws std::out_of_range 指定された行インデックスがテーブルの範囲外の場合
		 * @throws std::invalid_argument TSVのデータ形式が不正な場合
		 */
		static PlayerStatusParameter CreateParameterFromRow(const TSVTable& table, int rowIndex);


	private:
		static std::unordered_map<WeaponType, PlayerStatusParameter> m_table; //! 武器タイプとプレイヤーステータスパラメータのマッピングテーブル
	};
}
