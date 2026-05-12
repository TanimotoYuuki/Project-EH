#include "stdafx.h"
#include "MagicParameterTable.h"

namespace
{
}

namespace nsApp
{
	const std::unordered_map<nsActor::MagicType, MagicParameter> MagicParameterTable::m_magicTable =
	{
		/* 通常魔法: スピード 120(秒間), 寿命 1.5秒, ダメージ 10 */
		{ nsActor::MagicType::enNormalMagic, { 120.0f, 1.5f, 10.0f, 10.0f, 10.0f, 10.0f, Vector3::One * 0.5f, Quaternion::Identity, SearchMissileModelPath("NormalAttackMissile") } },

		/* 連打魔法: 少し寿命を長く設定 */
		{ nsActor::MagicType::enRushMagic,   { 140.0f, 2.0f, 8.0f,  10.0f, 10.0f, 15.0f, Vector3::One * 0.5f, Quaternion::Identity, SearchMissileModelPath("PressingAttackMissile") } },

		/* チャージ魔法: 威力3倍、当たり判定も大きく、生成位置をさらに前方に */
		{ nsActor::MagicType::enChargeMagic, { 180.0f, 2.5f, 30.0f, 20.0f, 10.0f, 20.0f, Vector3::One * 1.0f, Quaternion::Identity, SearchMissileModelPath("ChargeAttackMissile") } },

		/* 空中魔法: 基本は通常魔法と同じだが、ばら撒き用に調整可能 */
		{ nsActor::MagicType::enAirMagic,    { 120.0f, 1.2f, 10.0f, 10.0f, 12.0f, 15.0f, Vector3::One * 0.5f, Quaternion::Identity, SearchMissileModelPath("NormalAttackMissile") } },
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
	}


	Quaternion MagicParameterTable::MakeAngle(float angleX, float angleY, float angleZ)
	{
		/* 関数内でローカル変数を定義する。 */
		Quaternion localMakeAngleX, localMakeAngleY, localMakeAngleZ;
		localMakeAngleX.SetRotationDegX(angleX); //! X軸の回転角を設定。
		localMakeAngleY.SetRotationDegY(angleY); //! Y軸の回転角を設定。
		localMakeAngleZ.SetRotationDegZ(angleZ); //! Z軸の回転角を設定。

		/* 最終的な回転行列の計算結果を戻り値とする。*/
		return localMakeAngleX * localMakeAngleY * localMakeAngleZ;
	}
}