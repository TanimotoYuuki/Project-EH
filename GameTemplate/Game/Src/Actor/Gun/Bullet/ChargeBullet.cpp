#include "stdafx.h"
#include "ChargeBullet.h"
#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace nsApp
{
	namespace nsActor
	{
		bool ChargeBullet::Start()
		{
			/* チャージ弾の情報を取り出す。*/ 
			const auto& chargeBulletInfo = BulletModelRegister::GetBulletModelInfo(BulletType::enCharge);

			/* モデルクラスを生成。*/
			m_chargeBulletRender = std::make_unique<ModelRender>();

			/* モデルの情報を初期化。*/
			m_chargeBulletRender->Init(chargeBulletInfo.bulletModelName.c_str());

			/* 大きさを設定する。*/
			m_chargeBulletRender->SetScale(Vector3::One * chargeBulletInfo.bulletScale * 2.0f);

			/* 初期座標をセットする。*/
			m_chargeBulletRender->SetPosition(m_position);

			/* 進行方向を設定する。*/
			m_angle.SetRotationDegZ(-90.0f);
			m_chargeBulletRender->SetRotation(m_angle);

			return true;
		}


		void ChargeBullet::Update()
		{
			if (CheckLifeTime())
			{
				DeleteGO(this);
				return;
			}

			/* 角度を更新。*/
			m_chargeBulletRender->SetRotation(m_angle);

			/* 座標を更新する。*/
			UpdatebBulletPosition();
		}


		void ChargeBullet::Render(RenderContext& rc)
		{
			if (m_chargeBulletRender)
				m_chargeBulletRender->Draw(rc);
		}


		void ChargeBullet::UpdatebBulletPosition() 
		{
			m_position += m_direction * m_speed * g_gameTime->GetFrameDeltaTime();

			if (m_chargeBulletRender) 
			{
				m_chargeBulletRender->SetPosition(m_position);
				m_chargeBulletRender->Update();
			}
		}
	}
}