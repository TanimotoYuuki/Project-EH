#include "stdafx.h"
#include "DashBullet.h"
#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace nsApp
{
	namespace nsActor
	{
		bool DashBullet::Start()
		{
			/* 弾丸の情報を取りだす。*/
			const auto& bulletInfo = BulletModelRegister::GetBulletModelInfo(BulletType::enDash);

            /* 生成。*/
			m_dashBulletRender = std::make_unique<ModelRender>();

			/* モデルのセット。*/
			m_dashBulletRender->Init(bulletInfo.bulletModelName.c_str());

			/* 大きさの設定。*/
			m_dashBulletRender->SetScale(Vector3::One * bulletInfo.bulletScale * 2.0f);

			/* 初期座標を設定。*/
			m_dashBulletRender->SetPosition(m_position);

			m_angle.SetRotationDegZ(-90.0f);
			m_dashBulletRender->SetRotation(m_angle);

			return true;
		}


		void DashBullet::Update()
		{
			if (CheckLifeTime())
			{
				DeleteGO(this);
				return;
			}

			m_dashBulletRender->SetRotation(m_angle);
			/* 座標を更新*/
			UpdateBulletPosition();
		}

		void DashBullet::Render(RenderContext& rc)
		{
			if (m_dashBulletRender)
				m_dashBulletRender->Draw(rc);
		}


		void DashBullet::UpdateBulletPosition()
		{
			m_position += m_direction * m_speed * g_gameTime->GetFrameDeltaTime();

			if (m_dashBulletRender)
				m_dashBulletRender->SetPosition(m_position);

			m_dashBulletRender->Update();
		}
	}
}