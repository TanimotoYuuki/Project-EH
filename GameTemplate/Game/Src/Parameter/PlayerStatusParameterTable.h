#pragma once
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include <unordered_map>
#include <string>

namespace nsApp
{
	class TSVTable;

	/**
	 * @struct PlayerStatusParameter
	 * @detail プレイヤーのステータスパラメータを表す構造体。
	 */
	struct PlayerStatusParameter
	{
		int maxHP = 1000;					  //! プレイヤーの最大HP。
		float normalDamage = 40.0f;			  //! 通常攻撃のダメージ。基本ダメージとして表現。
		float criticalDamage = 1.5f;		  //! クリティカルダメージ倍率。基本ダメージに対する倍率で表現。
		float walkSpeed = 1.0f;				  //! 歩き速度の倍率。基本移動速度に対する倍率で表現。
		float runSpeed = 1.5f;				  //! 走り速度の倍率。基本移動速度に対する倍率で表現。
		float jumpPower = 500.0f;			  //! ジャンプ力。
		float airMoveSpeed = 120.0f;	      //! 空中移動速度。
		float gravity = 30.0f;				  //! 重力加速度。
		float maxFallVelocity = -1200.0f;	  //! 最大落下速度。（負の値で表現）
		float guardMaxDuration = 2.0f;        //! ガード最大時間。（秒）
		float guardDamageReduction = 0.5f;    //! ダメージ軽減率。（0.0〜1.0）
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
