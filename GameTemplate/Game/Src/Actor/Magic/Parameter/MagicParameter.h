#pragma once
/**
 * @file   MagicParameter.h
 * @brief  魔法のパラメータ構造体定義
 * @author Yamaguchi Hayato
 * @date   2026/06/03: 最終更新日。
 */


#include <string>

namespace nsApp
{
	/**
	 * @struct MagicParameter
	 * @brief 魔法のパラメータ構造体
	 */
	struct MagicParameter
	{
		float speedPerSecond = 0.0f;				//! 1秒あたりの移動距離。
		float lifeTime = 0.0f;						//! 存在時間。
		float damage = 0.0f;						//! ダメージ量。
		float healAmount = 0.0f;					//! 回復量。
		float radius = 0.0f;						//! 当たり判定の半径。
		float coolTime = 0.0f;						//! クールタイム。
		float spawnOffsetY = 0.0f;					//! 魔法の生成位置のY軸方向のオフセット。
		float forwardOffset = 0.0f;					//! 魔法の生成位置の前方方向のオフセット。
		Vector3 scale = Vector3::One;				//! 魔法のスケール。
		Quaternion angle = Quaternion::Identity;	//! 魔法の回転角。
		std::string modelPath;						//! 魔法のモデルパス。
		std::string effectName;						//! 魔法のエフェクト名。
	};
}
