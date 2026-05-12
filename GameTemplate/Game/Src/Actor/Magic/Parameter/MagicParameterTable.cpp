#include "stdafx.h"
#include "MagicParameterTable.h"

namespace
{
	Quaternion MakeAngle(float angleX, float angleY, float angleZ)
	{
		Quaternion localMakeAngleX, localMakeAngleY, localMakeAngleZ;
		localMakeAngleX.SetRotationDegX(angleX);
		localMakeAngleY.SetRotationDegY(angleY);
		localMakeAngleZ.SetRotationDegZ(angleZ);

		return localMakeAngleX * localMakeAngleY * localMakeAngleZ;
	}

	const auto MAGIC_ANGLE = MakeAngle(90.0f, 0.0f, 0.0f);
}

namespace nsApp
{
	const std::unordered_map<nsActor::MagicType, MagicParameter> MagicParameterTable::m_magicTable =
	{
		/* 通常魔法*/
		{ nsActor::MagicType::enNormalMagic, { 120.0f, 1.5f, 10.0f, 10.0f, 10.0f, 10.0f, Vector3::One * 0.5f, MAGIC_ANGLE, SearchMissileModelPath("NormalAttackMissile") } },

		/* 連打魔法*/
		{ nsActor::MagicType::enRushMagic,   { 140.0f, 2.0f, 8.0f,  10.0f, 10.0f, 15.0f, Vector3::One * 0.5f, MAGIC_ANGLE, SearchMissileModelPath("PressingAttackMissile") } },

		/* チャージ魔法。*/
		{ nsActor::MagicType::enChargeMagic, { 180.0f, 2.5f, 30.0f, 20.0f, 10.0f, 20.0f, Vector3::One * 1.0f, MAGIC_ANGLE, SearchMissileModelPath("ChargeAttackMissile") } },

		/* 空中魔法 */
		{ nsActor::MagicType::enAirMagic,    { 120.0f, 1.2f, 10.0f, 10.0f, 12.0f, 15.0f, Vector3::One * 0.5f, MAGIC_ANGLE, SearchMissileModelPath("NormalAttackMissile") } },
	};


	const MagicParameter& MagicParameterTable::GetParameter(nsActor::MagicType type)
	{
		/* テーブルの情報を探す。*/
		auto missileIterator = m_magicTable.find(type);

		/* 見つからない場合。*/
		if (missileIterator != m_magicTable.end())
		{
			/* 見つかった場合。*/
			return missileIterator->second;
		}

		/* 安全対策。*/
		return m_magicTable.at(nsActor::MagicType::enNormalMagic);
	}
}