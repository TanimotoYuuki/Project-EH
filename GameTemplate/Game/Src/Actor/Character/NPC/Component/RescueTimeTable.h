#pragma once
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include <unordered_map>
#include <string>

namespace nsApp
{
	class TSVTable;

	/**
	 * @brief RescueTimeTable.h
	 * @struct RescueTimeParameter
	 * @details 救助時間に関するパラメータを格納する構造体
	 */
	struct RescueTimeParameter
	{
		int requiredHelpTime = 20;	   //! 救助に必要な時間（秒）
		float rescueGaugeSpeed = 1.0f; //! 救助ゲージの増加速度（1.0fが基準）
		int helpSEInterval = 30;	   //! 救助SEの再生間隔（フレーム数）
	};

	class RescueTimeTable
	{
	public:
		static bool LoadTSVFile(const char* filePath);
		static const RescueTimeParameter& GetParameter(WeaponType weaponType);

	private:
		static WeaponType ConvertWeaponType(const std::string& weaponName);
		static RescueTimeParameter CreateParameterFromRow(const TSVTable& table, int rowIndex);

	private:
		static std::unordered_map<WeaponType, RescueTimeParameter> m_table;
	};
}
