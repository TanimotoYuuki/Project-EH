#include "stdafx.h"
#include "IGunBullet.h"
#include "Boss.h"

namespace
{
	const auto BOSS_CENTER_OFFSET_Y = 50.0f;            //! ボスの中心を狙うためのY軸オフセット
	const auto BOSS_HIT_DISTANCE_THRESHOLD = 150.0f;    //! 線分判定でヒットとする許容距離
}

namespace nsApp
{
	namespace nsActor
	{
		IGunBullet::~IGunBullet()
		{
			if (m_bulletCollider != nullptr) DeleteGO(m_bulletCollider);
		}

		void IGunBullet::Initialize(const BulletParameter& param, const Vector3& spawnPosition, const Vector3& forwardDirection)
		{
			m_param = param;
			m_position = spawnPosition;
			m_currentLifeTime = param.lifeTimeSecond;
			m_speedPerSecond = param.speedPerSecond;
			m_velocity = forwardDirection * m_speedPerSecond;

			m_direction.SetRotation(Vector3::Front, forwardDirection);
			m_angle = m_direction * param.angle;

			m_modelRender = std::make_unique<ModelRender>();
			m_modelRender->Init(param.modelName.c_str());
			m_modelRender->SetScale(param.scale);
			m_modelRender->SetRotation(m_angle);

			m_bulletCollider = NewGO<nsK2Engine::CollisionObject>(0, "BulletCollision");
			m_bulletCollider->CreateSphere(m_position, Quaternion::Identity, param.radius);
			m_bulletCollider->SetIsEnableAutoDelete(false);

			m_previousPosition = m_position;
		}

		void IGunBullet::Update()
		{
			m_deltaTime = g_gameTime->GetFrameDeltaTime();
			m_previousPosition = m_position;

			m_position += m_velocity * m_deltaTime;
			m_currentLifeTime -= m_deltaTime;

			if (m_currentLifeTime <= 0.0f)
			{
				DeleteGO(this);
				return;
			}

			if (m_param.type == BulletType::enRush) {
				TargetMoving();
			}

			if (m_bulletCollider != nullptr)
				m_bulletCollider->SetPosition(m_position);

			if (m_modelRender)
			{
				m_modelRender->SetPosition(m_position);
				m_modelRender->SetRotation(m_angle);
				m_modelRender->Update();
			}

			if (CheckHitBoss())
			{
				DeleteGO(this);
				return;
			}
		}

		void IGunBullet::Render(RenderContext& rc)
		{
			if (m_modelRender) 
				m_modelRender->Draw(rc);
		}


		bool IGunBullet::CheckHitBoss()
		{
			if (m_bulletCollider == nullptr)
				return false;

			auto boss = FindGO<nsActor::Boss>("boss"); 
			if (boss != nullptr && reinterpret_cast<uintptr_t>(boss) != 0xFFFFFFFFFFFFFFFF)
			{
				if (m_bulletCollider->IsHit(boss->GetController()))
				{
					boss->ApplyDamage(static_cast<int>(m_param.damage));
					return true;
				}

				m_bossPosition = boss->GetPosition();
				m_bossPosition.y += BOSS_CENTER_OFFSET_Y;

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

						if (m_distanceToBoss < BOSS_HIT_DISTANCE_THRESHOLD)
						{
							boss->ApplyDamage(static_cast<int>(m_param.damage));
							return true;
						}
					}
				}
			}
			return false;
		}
	}
}