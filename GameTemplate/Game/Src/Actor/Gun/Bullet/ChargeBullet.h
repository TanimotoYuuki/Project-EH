#pragma once
/**
 * @file   ChargeBullet.h
 * @brief  チャージ攻撃時に生成するクラス。
 * @author YamaguchiHayato
 * @date   2026/04/30
 */

#include "Src/Actor/Gun/Bullet/IGunBullet.h"

namespace nsApp
{
	namespace nsActor
	{
		class ChargeBullet : public IGunBullet
		{
		public:
			/* コンストラクタとデストラクタ。*/
			ChargeBullet() = default;
			virtual ~ChargeBullet() = default;


		public:
			/* ライフサイクル。*/
			bool Start() override;
			void Update() override;
			void Render(RenderContext& rc) override;


		private:
			void UpdatebBulletPosition();


		private:
			std::unique_ptr<ModelRender> m_chargeBulletRender = nullptr; //! チャージ弾のレンダー。
			Quaternion m_angle = Quaternion::Identity;                   //! 弾丸の回転角。
		};
	}
}

