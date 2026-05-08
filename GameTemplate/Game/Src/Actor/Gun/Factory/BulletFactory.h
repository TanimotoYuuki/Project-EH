#pragma once
/**
 * @file   BulletFactory.h
 * @brief  ’eŠÛ‚Ì‹¤’Ê¶¬‚ğŠÇ—‚·‚éƒNƒ‰ƒXB
 * @author Yamaguchi Hayato
 * @date   2026/05/05
 */

#include "Src/Actor/Gun/Register/BulletModelRegister.h"
#include "Src/Actor/Gun/Bullet/IGunBullet.h"

namespace nsApp
{
	class BulletFactory
	{
	public:
		static void CreateBullet(BulletType type, const Vector3& basePosition, const Vector3& direction);
	};
}


