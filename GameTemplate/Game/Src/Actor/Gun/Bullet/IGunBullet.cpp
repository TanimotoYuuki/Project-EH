#include "stdafx.h"
#include "IGunBullet.h"
#include "Boss.h"

namespace nsApp
{
	namespace nsActor
	{
		IGunBullet::~IGunBullet()
		{
			if (m_bulletCollider != nullptr)
				DeleteGO(m_bulletCollider);
		}

		void IGunBullet::InitCollision(Quaternion angle, float radius)
		{
			/* コリジョンを生成する。*/
			m_bulletCollider = NewGO<nsK2Engine::CollisionObject>(0, "BulletCollision");

		    /* コリジョンの情報を設定する。*/
			m_bulletCollider->CreateSphere(m_position, angle, radius);

			/* 自動削除を無効にする。*/
			m_bulletCollider->SetIsEnableAutoDelete(false);

			// 前回の座標を初期化
			m_previousPosition = m_position;
		}


		bool IGunBullet::CheckHitBoss()
		{
			/* 当たっていなければスルー。*/
			if (m_bulletCollider == nullptr)
				return false;

			auto boss = FindGO<nsActor::Boss>("boss");
			if (boss != nullptr && reinterpret_cast<uintptr_t>(boss) != 0xFFFFFFFFFFFFFFFF)
			{
				if (m_bulletCollider->IsHit(boss->GetController()))
					return true;

				m_bossPosition = boss->GetPosition();
				m_bossPosition.y += 50.0f; // ボスの高さに合わせて中心を狙う

				m_bulletTrajectory = m_position - m_previousPosition;
				m_vectorToBossTarget = m_bossPosition - m_previousPosition;

				m_trajectoryLengthSquared = m_bulletTrajectory.LengthSq();

				if (m_trajectoryLengthSquared > 0.0f)
				{
					m_closestPointRatio = m_vectorToBossTarget.Dot(m_bulletTrajectory) / m_trajectoryLengthSquared;

					if (m_closestPointRatio >= 0.0f && m_closestPointRatio <= 1.0f)
					{
						m_closestPointOnTrajectory = m_previousPosition + (m_bulletTrajectory * m_closestPointRatio);

						m_distanceToBoss = (m_bossPosition - m_closestPointOnTrajectory).Length();

						if (m_distanceToBoss < 150.0f)
						{
							boss->ApplyDamage(40.0f);
							return true;
						}
					}
				}
			}
			return false;
		}
	}
}