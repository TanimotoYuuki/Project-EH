#include "stdafx.h"
#include "AirBullet.h"
#include "Src/Actor/Gun/Register/BulletModelRegister.h"

namespace
{
	const auto BULLET_SCALE_MULTIPLIER = 2.0f; //! 弾のスケール倍率
	const auto BULLET_ANGLE_OFFSET_Z = -90.0f; //! 弾のZ軸回転オフセット
}

namespace nsApp
{
	namespace nsActor
	{
		bool AirBullet::Start()
		{
			/* 空中攻撃の弾の情報を取り出す。*/
			auto& bulletInfo = BulletModelRegister::GetBulletModelInfo(BulletType::enAirial);

			/* モデルを生成する。*/
			m_airBulletRender = std::make_unique<ModelRender>();
			/* モデルを初期化して定数化する。*/
			m_airBulletRender->Init(bulletInfo.bulletModelName.c_str());

			/* モデルの大きさを初期化する。*/
			m_airBulletRender->SetScale(Vector3::One * BULLET_SCALE_MULTIPLIER * bulletInfo.bulletScale);

			/* モデルの初期座標を設定する。*/
			m_airBulletRender->SetPosition(m_position);
			
			m_angle.SetRotationDegZ(BULLET_ANGLE_OFFSET_Z);
			/* モデルの角度を初期化する。*/
			m_airBulletRender->SetRotation(m_angle);

			InitCollision(m_angle, 8.0f);
			return true;
		}


		void AirBullet::Update()
		{
			/* 弾の生存時間をチェックする。*/ 
			if (CheckLifeTime())
			{
				DeleteGO(this);
				return;
			}

			if (m_direction.LengthSq() > 0.0f)
			{
				m_angle.SetRotation(Vector3::Front, m_direction);
				m_offsetAngle.SetRotationDegZ(BULLET_ANGLE_OFFSET_Z);
				m_angle *= m_offsetAngle;
			}

			/* 弾の座標を保存する。*/
			m_previousPosition = m_position;

			/* 弾の座標を更新する。*/ 
			UpdateBulletPosition();

			/* コリジョンの座標を更新。*/
			UpdateBulletCollisionPosition();

			/* ボスに当たったかチェック。*/
			if (CheckHitBoss())
			{
				//! @TODO: ダメージ処理。
				DeleteGO(this);
				return;
			}

		}


		void AirBullet::Render(RenderContext& rc)
		{
			if (m_airBulletRender)
				m_airBulletRender->Draw(rc);
		}


		void AirBullet::UpdateBulletPosition()
		{
			/* 移動速度の計算。*/
			m_position += m_direction * m_speed * g_gameTime->GetFrameDeltaTime();

			if (m_airBulletRender)
			{
				m_airBulletRender->SetPosition(m_position);
				m_airBulletRender->SetRotation(m_angle);
				m_airBulletRender->Update();
			}
		}
	}
}