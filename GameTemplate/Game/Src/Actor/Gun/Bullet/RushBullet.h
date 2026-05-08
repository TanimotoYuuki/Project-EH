#pragma once
/**
 * @file   RushBullet.h
 * @brief  ラッシュ攻撃時に生成するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/01
 */

#include "Src/Actor/Gun/Bullet/IGunBullet.h"

namespace nsApp
{
	namespace nsActor
	{
		class RushBullet : public IGunBullet
		{
		public:
			/* コンストラクタとデストラクタ。*/
			RushBullet() = default;
			virtual ~RushBullet() = default; 
			

		public:
			/* ライフサイクル。*/
			bool Start() override;
			void Update() override;
			void Render(RenderContext& rc) override;


		private:
			void UpdatebBulletPosition();


		private:
			std::unique_ptr<ModelRender> m_rushBulletRender = nullptr; //! ラッシュ弾のレンダー。
			Quaternion m_angle = Quaternion::Identity;                 //! 弾丸の回転角。
			Quaternion m_offsetAngle = Quaternion::Identity;           //! 弾丸の回転角のオフセット。
		};
	}
}
