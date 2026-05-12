#pragma once
/**
 * @file   MagicParameter.h
 * @brief  魔法のパラメータを管理するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/05/12
 */

namespace nsApp
{
	/**
	 * @struct MagicParameter
	 * @brief  魔法のパラメータをまとめた構造体。
	 * @detail 魔法の種類ごとにパラメーターが異なるため構造体を用いて管理する。
	 */
	struct MagicParameter
	{
		float speedPerSecond = 0.0f;				//! 射出速度。
		float lifeTime = 0.0f;						//! 生成時間。
		float damage = 0.0f;						//! ダメージ量。
		float radius = 0.0f;						//! 半径。
		float spawnOffsetY = 0.0f;					//! 生成時のY軸オフセット。
		float forwardOffset = 0.0f;					//! 射出方向へのオフセット。
		Vector3 scale = Vector3::One;		        //! 大きさ。
		Quaternion angle = Quaternion::Identity;    //! 角度。
		std::string modelPath;						//! モデル名。
	};
}
