#pragma once
/**
 * @file   BulletParameter.h
 * @brief  弾丸のパラメータを管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/07
 */

#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace nsApp
{
	/**
	 * @struct BulletParameter
	 * @brief  弾丸のパラメータを管理する構造体。
	 */
	struct BulletParameter
	{
		float speedPerSecond;        //! 弾丸の速度。
		float lifeTimeSecond;        //! 弾丸の描画時間。
		float spawnOffsetY;          //! 生成時のY軸オフセット。
		float spawnForwardOffset;    //! 生成時の前方オフセット。
	};
}

