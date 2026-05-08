#pragma once
/**
 * @file   HeavyBullet.h
 * @brief  ヘビーブレットを管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/01
*/

#include "Src/Actor/Gun/Bullet/IGunBullet.h"

namespace nsApp
{
	namespace nsActor
	{
		class HeavyBullet :public IGunBullet
		{
		public:
			/* コンストラクタとデストラクタ。*/
			HeavyBullet() = default;
			virtual ~HeavyBullet() = default;


		public:
			/* ライフサイクル。*/
			bool Start() override;
			void Update() override;
			void Render(RenderContext& rc) override;


		private:
			std::unique_ptr<ModelRender> m_explosionBulletRender; //! ヘビーブレットのモデルレンダラー。
			Quaternion m_angle;                                   //! ヘビーブレットの角度。
			Quaternion m_offset;								  //! ヘビーブレットの角度オフセット。
		};
	}
}
