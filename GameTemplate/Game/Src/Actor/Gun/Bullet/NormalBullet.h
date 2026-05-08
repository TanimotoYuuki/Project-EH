#pragma once

/**
 * @file   NormalBullet.h
 * @brief  通常攻撃時に生成するクラス。
 * @author YamaguchiHayato
 * @date   2026/04/30
 */

#include "Src/Actor/Gun/Bullet/IGunBullet.h"

namespace nsApp
{
	namespace nsActor
	{
		class NormalBullet : public IGunBullet
		{
		public:
			/* コンストラクタとデストラクタ。*/
			NormalBullet() = default;
			virtual ~NormalBullet() = default;


		public:
			/* ライフサイクル。*/
			bool Start() override;
			void Update() override;
			void Render(RenderContext& rc) override;


		private:
			/* 弾丸の座標を更新。*/
			void UpdatebBulletPosition();


		private:
			std::unique_ptr<ModelRender> m_normalBulletRender = nullptr; //! 通常弾のレンダー。

			Quaternion m_angle = Quaternion::Identity;                   //! 弾丸の回転角。
		};
	}
}

