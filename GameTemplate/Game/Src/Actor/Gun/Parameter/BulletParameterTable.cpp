#include "stdafx.h"
#include "BulletParameterTable.h"


namespace
{
	const auto UNIFIED_SPEED = 500.0f;
	const auto UNIFIED_LIFE = 1.0f;

	namespace nsNormalBullet
	{
		const auto SPEED = UNIFIED_SPEED;			   //! 通常弾の速度。
		const auto LIFE_TIME = UNIFIED_LIFE;	       //! 通常弾の生存時間。
		const auto OFFSET_Y = 10.0f;				   //! 通常弾の生成位置のY軸オフセット。
		const auto FORWARD_OFFSET = 10.0f;			   //! 通常弾の生成位置の前方オフセット。
	}

	namespace nsChargeBullet
	{
		const auto SPEED = UNIFIED_SPEED;				//! チャージ弾の速度。
		const auto LIFE_TIME = UNIFIED_LIFE;		    //! チャージ弾の生存時間。
		const auto OFFSET_Y = 10.0f;				    //! チャージ弾の生成位置のY軸オフセット。
		const auto FORWARD_OFFSET = 10.0f;				//! チャージ弾の生成位置の前方オフセット。
	}

	namespace nsRushBullet
	{
		const auto SPEED = UNIFIED_SPEED;				 //! 突進弾の速度。
		const auto LIFE_TIME = UNIFIED_LIFE;		     //! 突進弾の生存時間。
		const auto OFFSET_Y = 10.0f;					 //! 突進弾の生成位置のY軸オフセット。
		const auto FORWARD_OFFSET = 10.0f;				 //! 突進弾の生成位置の前方オフセット。
	}

	namespace nsEnExplosive
	{
		const auto SPEED = UNIFIED_SPEED;		         //! 爆発弾の速度。
		const auto LIFE_TIME = UNIFIED_LIFE;		     //! 爆発弾の生存時間。
		const auto OFFSET_Y = 20.0f;				     //! 爆発弾の生成位置のY軸オフセット。
		const auto FORWARD_OFFSET = 30.0f;				 //! 爆発弾の生成位置の前方オフセット。
	}

	namespace nsAirialBullet
	{
		const auto SPEED = UNIFIED_SPEED;		         //! 空中弾の速度。
		const auto LIFE_TIME = UNIFIED_LIFE;		     //! 空中弾の生存時間。
		const auto OFFSET_Y = 12.0f;					 //! 空中弾の生成位置のY軸オフセット。
		const auto FORWARD_OFFSET = 22.0f;			     //! 空中弾の生成位置の前方オフセット。
	}

	namespace nsDashBullet
	{
		const auto SPEED = UNIFIED_SPEED;				 //! ダッシュ弾の速度。
		const auto LIFE_TIME = UNIFIED_LIFE;		     //! ダッシュ弾の生存時間。
		const auto OFFSET_Y = 8.0f;						 //! ダッシュ弾の生成位置のY軸オフセット。
		const auto FORWARD_OFFSET = 18.0f;				 //! ダッシュ弾の生成位置の前方オフセット。
	}
}

namespace nsApp
{
	/* 各弾丸のパラメータを設定する。*/
	const std::unordered_map<BulletType, BulletParameter> BulletParameterTable::m_bulletParameterTable =
	{
		/* 通常弾。*/
		{ 
			BulletType::enNormal, 
		    {
			    nsNormalBullet::SPEED, 
				nsNormalBullet::LIFE_TIME, 
				nsNormalBullet::OFFSET_Y, 
				nsNormalBullet::FORWARD_OFFSET 
	        }
		},

		/* チャージ弾。*/
		{ 
			BulletType::enCharge,    
		    {
				nsChargeBullet::SPEED,
				nsChargeBullet::LIFE_TIME,
				nsChargeBullet::OFFSET_Y,
			    nsChargeBullet::FORWARD_OFFSET
	        }
		},

		/* 突進弾。*/
		{
			BulletType::enRush,     
		    {
				nsRushBullet::SPEED,
				nsRushBullet::LIFE_TIME, 
				nsRushBullet::OFFSET_Y,
				nsRushBullet::FORWARD_OFFSET 
	        }
		},

		/* 爆発弾。*/
		{
			BulletType::enExplosive, 
		    {
				nsEnExplosive::SPEED,
				nsEnExplosive::LIFE_TIME,
				nsEnExplosive::OFFSET_Y, 
				nsEnExplosive::FORWARD_OFFSET
	        }
		},

		/* 空中弾。*/
		{ 
			BulletType::enAirial,  
		    {
				nsAirialBullet::SPEED,
				nsAirialBullet::LIFE_TIME, 
				nsAirialBullet::OFFSET_Y,
				nsAirialBullet::FORWARD_OFFSET 
	        }
		},

		/* ダッシュ弾。*/
		{ 
			BulletType::enDash,      
		    {
				nsDashBullet::SPEED,
				nsDashBullet::LIFE_TIME,
				nsDashBullet::OFFSET_Y, 
				nsDashBullet::FORWARD_OFFSET 
	        }
		}
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