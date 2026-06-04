#pragma once
#include "MagicParameter.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include <unordered_map>
#include <string>

namespace nsApp
{
	class TSVTable;

	class MagicParameterTable
	{
	public:
		static bool LoadTSVFile(const char* filePath);
		static const MagicParameter& GetParameter(nsActor::MagicType type);
		static const MagicParameter& GetParameter(const std::string& magicTypeName);

		static inline const std::string SearchMissileModelPath(const std::string& modelName)
		{
			return "Assets/modelData/Character/Weapon/" + modelName + ".tkm";
		}

	private:
		static nsActor::MagicType ConvertMagicType(const std::string& magicName);
		static MagicParameter CreateParameterFromRow(const TSVTable& table, int rowIndex, const MagicParameter& baseParameter);
		static void CreateDefaultTable(std::unordered_map<nsActor::MagicType, MagicParameter>& table);
		static Quaternion MakeAngle(float angleX, float angleY, float angleZ);

	private:
		static std::unordered_map<nsActor::MagicType, MagicParameter> m_magicTable;
		static std::unordered_map<std::string, MagicParameter> m_magicNameTable;
	};
}
