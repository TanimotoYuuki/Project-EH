#include "stdafx.h"
#include "RushBullet.h"
#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace nsApp
{
	namespace nsActor
	{
		bool RushBullet::Start()
		{
			/* 乱射弾の情報を取り出す。*/
			auto& rushBulletInfo = BulletModelRegister::GetBulletModelInfo(BulletType::enRush);

			/* 生成。*/
			m_rushBulletRender = std::make_unique<ModelRender>();

			/* モデルを読み込む。*/
			m_rushBulletRender->Init(rushBulletInfo.bulletModelName.c_str());

			/* 大きさを設定する。*/
			m_rushBulletRender->SetScale(Vector3::One * rushBulletInfo.bulletScale * 2.0f);

			/* 初期座標をセットする。*/
			m_rushBulletRender->SetPosition(m_position);

			/* 弾の向きをセットする。*/ 
			m_rushBulletRender->SetRotation(m_angle);
			return true;
		}


		void RushBullet::Update()
		{
			if (CheckLifeTime())
			{
				DeleteGO(this);
				return;
			}

			if (m_direction.LengthSq() > 0.0)
			{
				m_angle.SetRotation(Vector3::Front, m_direction);
				m_offsetAngle.SetRotationDegZ(-90.0f);
				m_angle *= m_offsetAngle;
			}

			m_rushBulletRender->SetPosition(m_position);
			m_rushBulletRender->SetRotation(m_angle);
			UpdatebBulletPosition();
		}


		void RushBullet::Render(RenderContext& rc)
		{
			/* モデルを描画する。*/
			if (m_rushBulletRender)
				m_rushBulletRender->Draw(rc);
		}


		void RushBullet::UpdatebBulletPosition()
		{
			/* モデルの座標を更新する。*/
			m_position += m_direction * m_speed * g_gameTime->GetFrameDeltaTime();

			if (m_rushBulletRender)
			{
				m_rushBulletRender->SetPosition(m_position);
				m_rushBulletRender->Update();
			}
		}
	}
}