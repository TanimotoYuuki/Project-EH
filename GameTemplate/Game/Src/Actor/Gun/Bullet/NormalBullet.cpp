#include "stdafx.h"
#include "NormalBullet.h"
#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace nsApp
{
	namespace nsActor
	{
		bool NormalBullet::Start()
		{
			/* 通常弾の情報を取り出す。*/
			const auto& normalBulletInfo = BulletModelRegister::GetBulletModelInfo(BulletType::enNormal);

			/* 生成。*/
			m_normalBulletRender = std::make_unique<ModelRender>();

			/* モデルを読み込む。*/
			m_normalBulletRender->Init(normalBulletInfo.bulletModelName.c_str());

			/* 大きさを設定する。*/
			m_normalBulletRender->SetScale(Vector3::One * 2.0f * normalBulletInfo.bulletScale);

			/* 初期座標をセットする。*/
			m_normalBulletRender->SetPosition(m_position);

			/* 初期角度をセットする。*/
			m_angle.SetRotationDegZ(-90.0f);
			m_normalBulletRender->SetRotation(m_angle);

			return true;
		}


		void NormalBullet::Update()
		{
			/* 弾丸の寿命を管理する。*/
			if (CheckLifeTime())
			{
				DeleteGO(this);
				return;
			}
			/* 角度を設定。*/
			m_normalBulletRender->SetRotation(m_angle);

			/* 座標を更新。*/
			UpdatebBulletPosition();
		}


		void NormalBullet::UpdatebBulletPosition()
		{
			/* 弾丸の座標を移動する。*/
			m_position += m_direction * m_speed * g_gameTime->GetFrameDeltaTime();

			/* 座標を更新する。*/
			if (m_normalBulletRender)
			{
				m_normalBulletRender->SetPosition(m_position);
				m_normalBulletRender->Update();
			}
		}


		void NormalBullet::Render(RenderContext& rc)
		{
			if(m_normalBulletRender)
				m_normalBulletRender->Draw(rc);
		}
	}
}