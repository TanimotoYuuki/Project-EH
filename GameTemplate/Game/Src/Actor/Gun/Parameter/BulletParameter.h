#pragma once
/**
 * @file   BulletParameter.h
 * @brief  弾丸のパラメータを管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/07
 */

#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace nsApp
{
	/**
	 * @struct BulletParameter
	 * @brief  弾丸のパラメータを管理する構造体。
	 */
	struct BulletParameter
	{
		BulletType type;             //! 弾丸の種類。
		float speedPerSecond;        //! 弾丸の速度。
		float lifeTimeSecond;        //! 弾丸の描画時間。
		float damage;                //! 弾丸のダメージ量。
		float radius;                //! 弾丸の半径。
		float spawnOffsetY;          //! 生成時のY軸オフセット。
		float spawnForwardOffset;    //! 生成時の前方オフセット。
		Vector3 scale;               //! 弾丸のスケール。
		Quaternion angle;            //! 弾丸の角度。
		std::string modelName;       //! 弾丸のモデル名。
	};
}

