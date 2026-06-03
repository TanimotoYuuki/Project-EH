#pragma once
/**
 * @file   BulletFactory.h
 * @brief  弾丸の共通生成を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/29
 */

#include "Src/Actor/Gun/Register/BulletModelRegister.h"
#include "Src/Actor/Gun/Bullet/IGunBullet.h"

namespace nsApp
{
	class BulletFactory
	{
	public:
		/**
		 * @brief 弾丸の生成を行う関数。
		 * @param type 弾丸の種類。
		 * @param basePosition 弾丸の生成位置。
		 * @param direction 弾丸の発射方向。
		 */
		static void CreateBullet(BulletType type, const Vector3& basePosition, const Vector3& direction);
	};
}


