#include "stdafx.h"
#include "BulletFactory.h"

#include "Src/Actor/Gun/Parameter/BulletParameterTable.h"

#include "Src/Actor/Gun/Bullet/NormalBullet.h"
#include "Src/Actor/Gun/Bullet/ChargeBullet.h"
#include "Src/Actor/Gun/Bullet/RushBullet.h"
#include "Src/Actor/Gun/Bullet/HeavyBullett.h"
#include "Src/Actor/Gun/Bullet/AirBullet.h"
#include "Src/Actor/Gun/Bullet/DashBullet.h"


namespace nsApp
{
	void BulletFactory::CreateBullet(BulletType type, const Vector3& basePosition, const Vector3& direction)
	{
		/* テーブルから弾丸のモデルの情報を取り出す。*/
		const auto& bulletParam = BulletParameterTable::GetParameter(type);

		/* 生成時の方向ベクトルを正規化。*/
		Vector3 normalizedDirection = direction;
		normalizedDirection.Normalize();

		/* 生成座標を計算。*/
		Vector3 spawnPosition = basePosition;
		spawnPosition.y += bulletParam.spawnOffsetY;
		spawnPosition += normalizedDirection * bulletParam.spawnForwardOffset;

		/* 弾丸の種類に応じて生成。*/
		nsActor::IGunBullet* bullet = nullptr;
		switch (type)
		{
		case BulletType::enNormal:
			bullet = NewGO<nsActor::NormalBullet>(0, "NormalBullet");
			break;

		case BulletType::enCharge:
			bullet = NewGO<nsActor::ChargeBullet>(0, "ChargeBullet");
			break;

		case BulletType::enRush:
			bullet = NewGO<nsActor::RushBullet>(0, "RushBullet");
			break;

		case BulletType::enExplosive:
			bullet = NewGO<nsActor::HeavyBullet>(0, "HeavyBullet");
			break;

		case BulletType::enAirial:
			bullet = NewGO<nsActor::AirBullet>(0, "AirBullet");
			break;

		case BulletType::enDash:
			bullet = NewGO<nsActor::DashBullet>(0, "DashBullet");
			break;

		default:
			return;
		}

		/* 生成に成功している場合、初期化。*/
		if (bullet)
			bullet->InitializeBullet(spawnPosition, normalizedDirection, bulletParam.speedPerSecond, bulletParam.lifeTimeSecond);
	}
}