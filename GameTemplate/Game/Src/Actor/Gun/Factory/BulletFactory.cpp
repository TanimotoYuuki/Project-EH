#include "stdafx.h"
#include "BulletFactory.h"
#include "Src/Actor/Gun/Parameter/BulletParameterTable.h"


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

		/* 未使用の弾丸を探す。*/
		nsActor::IGunBullet* bullet = nullptr;

		const auto& bullets = FindGOs<nsActor::IGunBullet>("Bullet");
		for (auto* pooledBullet : bullets)
		{
			if (pooledBullet != nullptr && !pooledBullet->IsInUse())
			{
				bullet = pooledBullet;
				break;
			}
		}

		/* 未使用の弾丸がなければ新規生成する。*/
		if (bullet == nullptr)
			bullet = NewGO<nsActor::IGunBullet>(0, "Bullet");

		/* 取得に成功している場合、初期化。*/
		if (bullet != nullptr)
			bullet->Initialize(bulletParam, spawnPosition, normalizedDirection);
	}
}