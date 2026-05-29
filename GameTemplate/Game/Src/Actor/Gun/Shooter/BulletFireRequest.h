#pragma once

/**
 * @file   BulletFireRequest.h
 * @brief  発射する弾丸のリクエスト情報をまとめる構造体。
 */

#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace nsApp
{
	/**
	 * @struct BulletFireRequest
	 * @brief 発射する弾丸のリクエスト情報をまとめる構造体。
	 */
	struct BulletFireRequest
	{
		BulletType kind = BulletType::enNone; 	   //! 発射する弾丸の種類。
		Vector3 basePosition = Vector3::Zero;	   //! 発射する弾丸の生成位置。
		Vector3 direction = Vector3::Zero;		   //! 発射する弾丸の方向。
	};
}
