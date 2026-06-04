#include "stdafx.h"
#include "MagicParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
	std::unordered_map<nsActor::MagicType, MagicParameter> MagicParameterTable::m_magicTable;
	std::unordered_map<std::string, MagicParameter> MagicParameterTable::m_magicNameTable;

	bool MagicParameterTable::LoadTSVFile(const char* filePath)
	{
		std::unordered_map<nsActor::MagicType, MagicParameter> loadedTable;
		CreateDefaultTable(loadedTable);

		TSVTable table;
		if (!table.LoadTSVFile(filePath))
		{
			m_magicTable.swap(loadedTable);
			return false;
		}

		std::unordered_map<std::string, MagicParameter> loadedNameTable;

		for (int rowIndex = 0; rowIndex < table.GetRowCount(); ++rowIndex)
		{
			const std::string magicName = table.GetString(rowIndex, "MagicType");
			if (magicName.empty())
				continue;

			const nsActor::MagicType magicType = ConvertMagicType(magicName);

			MagicParameter baseParameter;
			if (magicType != nsActor::MagicType::enNone)
				baseParameter = loadedTable[magicType];

			const MagicParameter parameter = CreateParameterFromRow(table, rowIndex, baseParameter);
			loadedNameTable[magicName] = parameter;

			if (magicType != nsActor::MagicType::enNone)
				loadedTable[magicType] = parameter;
		}

		m_magicTable.swap(loadedTable);
		m_magicNameTable.swap(loadedNameTable);
		return true;
	}

	const MagicParameter& MagicParameterTable::GetParameter(nsActor::MagicType type)
	{
		if (m_magicTable.empty())
			CreateDefaultTable(m_magicTable);

		auto iterator = m_magicTable.find(type);
		if (iterator != m_magicTable.end())
			return iterator->second;

		return m_magicTable[nsActor::MagicType::enNormalMagic];
	}

	const MagicParameter& MagicParameterTable::GetParameter(const std::string& magicTypeName)
	{
		auto iterator = m_magicNameTable.find(magicTypeName);
		if (iterator != m_magicNameTable.end())
			return iterator->second;

		static const MagicParameter fallbackParameter;
		return fallbackParameter;
	}

	nsActor::MagicType MagicParameterTable::ConvertMagicType(const std::string& magicName)
	{
		if (magicName == "Normal" || magicName == "NormalMagic" || magicName == "enNormalMagic")
			return nsActor::MagicType::enNormalMagic;

		if (magicName == "Rush" || magicName == "RushMagic" || magicName == "enRushMagic")
			return nsActor::MagicType::enRushMagic;

		if (magicName == "Charge" || magicName == "ChargeMagic" || magicName == "enChargeMagic")
			return nsActor::MagicType::enChargeMagic;

		if (magicName == "Air" || magicName == "AirMagic" || magicName == "enAirMagic")
			return nsActor::MagicType::enAirMagic;

		return nsActor::MagicType::enNone;
	}

	MagicParameter MagicParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex, const MagicParameter& baseParameter)
	{
		MagicParameter parameter = baseParameter;

		parameter.damage = table.GetFloat(rowIndex, "Power", table.GetFloat(rowIndex, "Damage", parameter.damage));
		parameter.healAmount = table.GetFloat(rowIndex, "HealAmount", parameter.healAmount);
		parameter.radius = table.GetFloat(rowIndex, "Radius", parameter.radius);
		parameter.lifeTime = table.GetFloat(rowIndex, "LifeTime", parameter.lifeTime);
		parameter.coolTime = table.GetFloat(rowIndex, "CoolTime", parameter.coolTime);
		parameter.scale = Vector3(
			table.GetFloat(rowIndex, "ScaleX", parameter.scale.x),
			table.GetFloat(rowIndex, "ScaleY", parameter.scale.y),
			table.GetFloat(rowIndex, "ScaleZ", parameter.scale.z)
		);
		parameter.effectName = table.GetString(rowIndex, "EffectName", parameter.effectName.c_str());

		return parameter;
	}

	void MagicParameterTable::CreateDefaultTable(std::unordered_map<nsActor::MagicType, MagicParameter>& table)
	{
		const Quaternion magicAngle = MakeAngle(90.0f, 0.0f, 0.0f);

		table.clear();
		table[nsActor::MagicType::enNormalMagic] = { 120.0f, 1.5f, 10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 10.0f, Vector3::One * 0.5f, magicAngle, SearchMissileModelPath("NormalAttackMissile"), "" };
		table[nsActor::MagicType::enRushMagic] = { 140.0f, 2.0f, 8.0f, 0.0f, 10.0f, 0.0f, 10.0f, 15.0f, Vector3::One * 0.5f, magicAngle, SearchMissileModelPath("PressingAttackMissile"), "" };
		table[nsActor::MagicType::enChargeMagic] = { 180.0f, 2.5f, 30.0f, 0.0f, 20.0f, 0.0f, 10.0f, 20.0f, Vector3::One * 1.0f, magicAngle, SearchMissileModelPath("ChargeAttackMissile"), "" };
		table[nsActor::MagicType::enAirMagic] = { 120.0f, 1.2f, 10.0f, 0.0f, 10.0f, 0.0f, 12.0f, 15.0f, Vector3::One * 0.5f, magicAngle, SearchMissileModelPath("NormalAttackMissile"), "" };
	}

	Quaternion MagicParameterTable::MakeAngle(float angleX, float angleY, float angleZ)
	{
		Quaternion localMakeAngleX, localMakeAngleY, localMakeAngleZ;
		localMakeAngleX.SetRotationDegX(angleX);
		localMakeAngleY.SetRotationDegY(angleY);
		localMakeAngleZ.SetRotationDegZ(angleZ);

		return localMakeAngleX * localMakeAngleY * localMakeAngleZ;
	}
}
