#include "stdafx.h"
#include "BulletParameterTable.h"


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

	const auto UNIFIED_SPEED = 500.0f;								    //! 共通の射出速度。 
	const auto UNIFIED_LIFE = 1.0f;									    //! 共通の描画時間。
	const auto UNIFIED_SCALE = Vector3::One * 1.0f;						//! 共通のスケール。

	const auto BULLET_ANGLE = MakeAngle(0.0f, 0.0f, -90.0f);            // ! 弾丸の基本的な角度。モデルの向きに合わせて設定。
	const auto HEAVY_ANGLE = MakeAngle(90.0f, 0.0f, 0.0f);
}


namespace nsApp
{
	/* 各弾丸のパラメータを設定する。*/
	const std::unordered_map<BulletType, BulletParameter> BulletParameterTable::m_bulletParameterTable =
	{
		{ BulletType::enNormal,    { BulletType::enNormal,    UNIFIED_SPEED, UNIFIED_LIFE, 10.0f, 15.0f, 10.0f, 10.0f, UNIFIED_SCALE, BULLET_ANGLE, GetModelPath("NormalBullet") } },
		{ BulletType::enCharge,    { BulletType::enCharge,    UNIFIED_SPEED, UNIFIED_LIFE, 30.0f, 20.0f, 10.0f, 10.0f, UNIFIED_SCALE, BULLET_ANGLE, GetModelPath("ChargeBullet") } },
		{ BulletType::enRush,      { BulletType::enRush,      UNIFIED_SPEED, UNIFIED_LIFE, 8.0f,  15.0f, 10.0f, 10.0f, UNIFIED_SCALE, BULLET_ANGLE, GetModelPath("NormalBullet") } },
		{ BulletType::enExplosive, { BulletType::enExplosive, UNIFIED_SPEED, UNIFIED_LIFE, 50.0f, 40.0f, 20.0f, 30.0f, UNIFIED_SCALE, HEAVY_ANGLE,  GetModelPath("ExplosionBullet") } },
		{ BulletType::enAirial,    { BulletType::enAirial,    UNIFIED_SPEED, UNIFIED_LIFE, 10.0f, 15.0f, 12.0f, 22.0f, UNIFIED_SCALE, BULLET_ANGLE, GetModelPath("AirBullet") } },
		{ BulletType::enDash,      { BulletType::enDash,      UNIFIED_SPEED, UNIFIED_LIFE, 15.0f, 20.0f, 8.0f,  18.0f, UNIFIED_SCALE, BULLET_ANGLE, GetModelPath("DashBullet") } }
	}; 

	const BulletParameter& BulletParameterTable::GetParameter(BulletType type)
	{
		/* 弾丸の情報を探索する。*/
		auto bulletIterator = m_bulletParameterTable.find(type);

		/* 見つからなかった場合はエラーを出す。*/
		if (bulletIterator != m_bulletParameterTable.end())
			return bulletIterator->second;

		return m_bulletParameterTable.at(BulletType::enNormal);
	}


}