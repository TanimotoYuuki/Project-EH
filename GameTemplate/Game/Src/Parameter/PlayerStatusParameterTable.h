#pragma once

#include <string>
#include <unordered_map>

#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	/**
	 * @struct PlayerStatusParameter
	 * @brief Playerの状態パラメータ。
	 */
	struct PlayerStatusParameter
	{
		int maxHP;				 //! Playerの最大HP。
		float normalDamage;		 //! Playerの通常攻撃のダメージ。
		float criticalDamage;	 //! Playerのクリティカル攻撃のダメージ。
		float criticalRate;      //! Playerのクリティカル率。

		float walkSpeed;		 //! Playerの歩行速度。 
		float runSpeed;		     //! Playerの走行速度。
		float jumpPower;		 //! Playerのジャンプ力。
		float airMoveSpeed;		 //! Playerの空中での移動速度。
		float gravity;			 //! Playerの重力。ジャンプ力や落下速度に影響する。
		float maxFallVelocity;   //! Playerの最大落下速度。落下速度がこの値を超えないようにする。
	};


	class TSVTable;
	class PlayerStatusParameterTable
	{
	public:
		/**
		 * @brief TSVファイルの読み込み処理。
		 * @param filePath TSVファイルのパス。
		 * @return TSVファイルの読み込みに成功した場合はtrue。
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief WeaponTypeに対応するPlayerStatusParameterを取得する。
		 * @param weaponType WeaponType。Playerの状態パラメータを取得するためのキー。
		 * @return WeaponTypeに対応するPlayerStatusParameter。WeaponTypeに対応するPlayerStatusParameterが存在しない場合は、デフォルトのPlayerStatusParameterを返す。
		 */
		static const PlayerStatusParameter& GetParameter(WeaponType weaponType);


	private:
		/**
		 * @brief TSVの文字列をWeaponTypeに変換する。
		 * @param weaponName TSVの文字列。WeaponTypeに変換するためのキー。
		 * @return TSVの文字列に対応するWeaponType。TSVの文字列に対応するWeaponTypeが存在しない場合は、WeaponType::Noneを返す。
		 */
		static WeaponType ConvertWeaponType(const std::string& weaponName);

		/**
		 * @brief TSVの1行からPlayerStatusParameterを作成する。
		 * @param table TSVテーブル。PlayerStatusParameterを作成するためのデータが格納されているテーブル。
		 * @param rowIndex 行インデックス。PlayerStatusParameterを作成するためのデータが格納されている行のインデックス。
		 * @return	
		 */
		static PlayerStatusParameter CreateParameterFromRow(const TSVTable& table, int rowIndex);


	private:
		static std::unordered_map<WeaponType, PlayerStatusParameter> m_table; //! WeaponTypeに対応するPlayerStatusParameterのテーブル。
	};
}