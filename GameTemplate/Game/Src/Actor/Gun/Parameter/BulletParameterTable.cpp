#include "stdafx.h"
#include "BulletParameterTable.h"
#include "Src/Utilty/TSVTableLoder.h"

namespace
{
	/**
	 * @brief 弾丸の角度を作成する。
	 * @param angleX X軸の角度。
	 * @param angleY Y軸の角度。
	 * @param angleZ Z軸の角度。
	 * @return 回転行列の計算結果。
     */
	Quaternion MakeAngle(float angleX, float angleY, float angleZ)
	{
		/* 関数内でローカル変数を定義する。 */
		Quaternion localMakeAngleX, localMakeAngleY, localMakeAngleZ;
		localMakeAngleX.SetRotationDegX(angleX);						//! X軸の回転角を設定。
		localMakeAngleY.SetRotationDegY(angleY);					    //! Y軸の回転角を設定。
		localMakeAngleZ.SetRotationDegZ(angleZ);						//! Z軸の回転角を設定。

		/* 最終的な回転行列の計算結果を戻り値とする。*/
		return localMakeAngleX * localMakeAngleY * localMakeAngleZ;
	}
}


namespace nsApp
{
	std::unordered_map<BulletType, BulletParameter> BulletParameterTable::m_bulletParameterTable;


	bool BulletParameterTable::LoadTSVFile(const char* filePath)
	{
		/* TSVTableLoderを使って、TSVからBulletParameterのテーブルを作成する。 */
		return TSVTableLoder::LoadTable<BulletType, BulletParameter>(
			filePath,
			"Type",
			m_bulletParameterTable,
			BulletParameterTable::ConvertBulletType,
			[](const TSVTable& table, int rowIndex, BulletType bulletType)
			{
				/* TSVの1行からBulletParameterを作成する。 */
				return BulletParameterTable::CreateParameterFromRow(table, rowIndex, bulletType);
			}
		);
	}


	BulletParameter BulletParameterTable::CreateParameterFromRow(const TSVTable& table, int rowIndex, BulletType bulletType)
	{
		/* 弾丸のスケールを作成。*/
		const Vector3 scale = Vector3(
			table.GetFloat(rowIndex, "ScaleX", 1.0f),
			table.GetFloat(rowIndex, "ScaleY", 1.0f),
			table.GetFloat(rowIndex, "ScaleZ", 1.0f)
		);

		/* 弾丸の回転行列を作成。*/
		const Quaternion angle = MakeAngle(
			table.GetFloat(rowIndex, "AngleX", 0.0f),
			table.GetFloat(rowIndex, "AngleY", 0.0f),
			table.GetFloat(rowIndex, "AngleZ", 0.0f)
		);

		/* 弾丸のモデル名を取得。*/
		const std::string modelName = table.GetString(rowIndex, "ModelName", "NormalBullet");

		/* TSVファイル内のデータから弾丸パラメータを作成。*/
		return BulletParameter
		{
			bulletType,
			table.GetFloat(rowIndex, "Speed", 500.0f),
			table.GetFloat(rowIndex, "LifeTime", 1.0f),
			table.GetFloat(rowIndex, "Damage", 10.0f),
			table.GetFloat(rowIndex, "Radius", 15.0f),
			table.GetFloat(rowIndex, "SpawnOffsetY", 10.0f),
			table.GetFloat(rowIndex, "SpawnForwardOffset", 10.0f),
			scale,
			angle,
			GetModelPath(modelName)
		};
	}


	BulletType BulletParameterTable::ConvertBulletType(const std::string& typeName)
	{
		/* 通常弾。*/
		if (typeName == "Normal" || typeName == "enNormal" || typeName == "BulletType::enNormal")
			return BulletType::enNormal;

		/* チャージ弾。*/ 
		if (typeName == "Charge" || typeName == "enCharge" || typeName == "BulletType::enCharge")
			return BulletType::enCharge;

		/* ラッシュ弾。*/
		if (typeName == "Rush" || typeName == "enRush" || typeName == "BulletType::enRush")
			return BulletType::enRush;

		/* 爆発弾。*/
		if (typeName == "Explosive" || typeName == "enExplosive" || typeName == "BulletType::enExplosive")
			return BulletType::enExplosive;

		/* 空中弾。*/
		if (typeName == "Air" || typeName == "Airial" || typeName == "enAir" || typeName == "enAirial" || typeName == "BulletType::enAir" || typeName == "BulletType::enAirial")
			return BulletType::enAirial;

		/* ダッシュ弾。*/
		if (typeName == "Dash" || typeName == "enDash" || typeName == "BulletType::enDash")
			return BulletType::enDash;

		return BulletType::enNormal;
	}


	const BulletParameter& BulletParameterTable::GetParameter(BulletType type)
	{
		/* 弾丸の情報を探索する。*/
		auto bulletIterator = m_bulletParameterTable.find(type);

		/* 見つからなかった場合はエラーを出す。*/
		if (bulletIterator != m_bulletParameterTable.end())
			return bulletIterator->second;

		/* TSV読み込み前、または読み込み失敗時の保険を返す。*/
		static const BulletParameter fallbackParameter =
		{
			BulletType::enNormal,
			500.0f,
			1.0f,
			10.0f,
			15.0f,
			10.0f,
			10.0f,
			Vector3::One,
			MakeAngle(0.0f, 0.0f, -90.0f),
			BulletParameterTable::GetModelPath("NormalBullet")
		};

		return fallbackParameter;
	}
}