#include "stdafx.h"
#include "HeavyBullett.h"
#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace
{
	const auto BULLED_ANGLE_DOWN_X = 90.0f; //! 弾を下に向けるX軸角度
}

namespace nsApp
{
	namespace nsActor
	{
		bool HeavyBullet::Start()
		{
            // 重攻撃用の enExplosive モデル情報を取得
            const auto& bulletInfo = BulletModelRegister::GetBulletModelInfo(BulletType::enExplosive);

			m_explosionBulletRender = std::make_unique<ModelRender>();
			m_explosionBulletRender->Init(bulletInfo.bulletModelName.c_str());

			m_explosionBulletRender->SetScale(Vector3::One * bulletInfo.bulletScale * 3.0f);
			m_explosionBulletRender->SetPosition(m_position);

            // 進行方向に向けて寝かせる
            m_angle.SetRotation(Vector3::Front, m_direction);
			m_offset.SetRotationDegX(BULLED_ANGLE_DOWN_X);
			m_angle *= m_offset;
			m_explosionBulletRender->SetRotation(m_angle);
			return true;
		}


		void HeavyBullet::Update()
		{
			if(CheckLifeTime())
			{
				DeleteGO(this);
				return;
			}
			m_position += m_direction * m_speed * g_gameTime->GetFrameDeltaTime();

			m_explosionBulletRender->SetPosition(m_position);
			m_explosionBulletRender->SetRotation(m_angle);

			m_explosionBulletRender->Update();
		}


		void HeavyBullet::Render(RenderContext& rc)
		{
			if(m_explosionBulletRender)
				m_explosionBulletRender->Draw(rc);
		}
	}
}