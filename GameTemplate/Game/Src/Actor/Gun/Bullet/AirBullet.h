#pragma once
/**
 * @file   AirBullet.h
 * @brief  空中攻撃の弾を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/01
 */

#include "Src/Actor/Gun/Bullet/IGunBullet.h"

namespace nsApp
{
	namespace nsActor
	{
		class AirBullet : public IGunBullet
		{
		public:
			/* コンストラクタとデストラクタ。*/
			AirBullet() = default;
			virtual ~AirBullet() = default;


		public:
			/* ライフサイクル。*/
			bool Start() override;
			void Update() override;
			void Render(RenderContext& rc) override;


		private:
			/* 座標を更新。*/
			void UpdateBulletPosition();


		private:
			std::unique_ptr<ModelRender> m_airBulletRender;    //! 空中攻撃の弾のモデルレンダラー。
			Quaternion m_angle = Quaternion::Identity;         //! 空中攻撃の弾の角度。
			Quaternion m_offsetAngle = Quaternion::Identity;   //! 空中攻撃の弾のオフセット角度。
		};
	}
}

