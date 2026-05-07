#include "stdafx.h"
#include "BulletModelRegister.h"

namespace nsApp
{
	const std::unordered_map<BulletType, BulletModelInfo> BulletModelRegister::m_bulletTable =
	{
		/* 通常弾の場合。*/
		{ BulletType::enNormal, { GetBulletModelFilePath("NormalBullet"), 0.5f} },

		/* チャージ弾の場合。*/
		{ BulletType::enCharge, { GetBulletModelFilePath("ChargeBullet"), 0.5f}},

		/* 爆発弾(重攻撃)の場合。*/
		{ BulletType::enExplosive, {GetBulletModelFilePath("ExplosionBullet"), 0.5f}},

		/* ダッシュ攻撃の場合。*/
		{ BulletType::enDash, {GetBulletModelFilePath("DashBullet"), 0.5f}},

		/* 空中攻撃弾。*/
		{ BulletType::enAirial, {GetBulletModelFilePath("AirBullet"), 0.5}},

		/* */
	};


	const BulletModelInfo& BulletModelRegister::GetBulletModelInfo(BulletType type)
	{
		/* テーブルの中から必要な弾丸の種類を出す。*/
		auto bulletIterator = m_bulletTable.find(type);
		if(bulletIterator != m_bulletTable.end())
			return bulletIterator->second;

		/* 範囲外アクセスの場合。*/
		return m_bulletTable.at(BulletType::enNormal);
	}
}