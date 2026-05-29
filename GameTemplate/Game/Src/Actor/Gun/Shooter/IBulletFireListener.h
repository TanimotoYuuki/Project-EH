#pragma once
/*
 * @file   IBulletFireListener.h
 * @brief  SEやエフェクトなど弾丸の発砲を検知。
 * @author Yamaguchi Hayato
 * @date   2026/05/07
 */

#include "Src/Actor/Gun/Shooter/BulletFireRequest.h"

namespace nsApp
{
	class IBulletFireListener
	{
	public:
		/* デストラクタ。*/
		virtual ~IBulletFireListener() = default;


	public:
		/**
		 * @brief 弾丸が発射した瞬間にコール。
		 * @param request 発射のリクエスト。
		 */
		virtual void OnBulletFired(const BulletFireRequest& request) = 0;
	};
}


