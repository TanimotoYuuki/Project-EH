#pragma once
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include <unordered_map>
#include <string>

namespace nsApp
{
	class TSVTable;

	struct AttackParameter
	{
		float damageMultiplier = 1.0f;
		float criticalRatel = 0.0f;
		float knockBackPower = 8.0f;
		float range = 80.0f;
		float radius = 40.0f;
		int hitStopFrame = 8;
		int coolTimer = 0;
		int comboNextTime = 10;
	};

	class AttackParameterTable
	{
	public:
		/**
		 * @brief TSVファイルから攻撃パラメータを読み込む
		 * @param filePath TSVファイルのパス
		 * @return 読み込み成功ならtrue、失敗ならfalse
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 攻撃タイプに対応する攻撃パラメータを取得する
		 * @param attackType 攻撃タイプ
		 * @return 攻撃パラメータの参照
		 * @note 指定された攻撃タイプが存在しない場合、デフォルトの攻撃パラメータを返す
		 */
		static const AttackParameter& GetAttackParameter(AttackType attackType);


	private:
		/**
		 * @brief 攻撃名を攻撃タイプに変換する
		 * @param attackName 攻撃名
		 * @return 攻撃タイプ
		 * @note 指定された攻撃名が存在しない場合、AttackType::enNoneを返す
		 */
		static AttackType ConvertAttackType(const std::string& attackName);

		/**
		 * @brief TSVテーブルの行から攻撃パラメータを作成する
		 * @param table TSVテーブル
		 * @param rowIndex 行インデックス
		 * @param attackType 攻撃タイプ
		 * @return 攻撃パラメータ
		 * @note TSVテーブルの行に必要なデータが不足している場合、デフォルトの攻撃パラメータを返す
		 */
		static AttackParameter CreateParameterFromRow(const TSVTable& table, int rowIndex, AttackType attackType);

		/**
		 * @brief 攻撃タイプに対応するデフォルトの攻撃パラメータを取得する
		 * @param attackType 攻撃タイプ
		 * @return デフォルトの攻撃パラメータの参照
		 * @note 指定された攻撃タイプが存在しない場合、AttackType::enNoneのデフォルトパラメータを返す
		 */
		static const AttackParameter& GetDefaultParameter(AttackType attackType);


	private:
		static std::unordered_map<AttackType, AttackParameter> m_table; //! 攻撃タイプに対応する攻撃パラメータのテーブル
	};
}
