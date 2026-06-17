#include "stdafx.h"
#include "IGunBullet.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"

namespace
{
	const auto BOSS_CENTER_OFFSET_Y = 50.0f;            //! �{�X�̒��S��_�����߂�Y���I�t�Z�b�g
	const auto BOSS_HIT_DISTANCE_THRESHOLD = 150.0f;    //! ��������Ńq�b�g�Ƃ��鋖�e����
	const auto DAMAGE_TEXT_OFFSET_Y = 120.0f;			//! �_���[�W�e�L�X�g�̕\���ʒu���{�X�̒��S����ǂꂾ����ɂ��邩�̃I�t�Z�b�g
}

namespace nsApp
{
	namespace nsActor
	{
		IGunBullet::~IGunBullet()
		{
			/* �폜����B*/
			if (m_bulletCollider != nullptr)
				DeleteGO(m_bulletCollider);
		}


		void IGunBullet::Initialize(const BulletParameter& param, const Vector3& spawnPosition, const Vector3& forwardDirection)
		{
			/* ��d�g�p��h�����ߏ������B*/
			m_isInUse = true;
			/* �p�����[�^�̃Z�b�g�A�b�v�B*/
			m_param = param;
			m_position = spawnPosition;
			m_previousPosition = spawnPosition;
			m_currentLifeTime = param.lifeTimeSecond;
			m_speedPerSecond = param.speedPerSecond;
			m_velocity = forwardDirection * m_speedPerSecond;

			/* �e�ۂ̌������v�Z�B*/
			m_direction.SetRotation(Vector3::Front, forwardDirection);
			m_angle = m_direction * param.angle;

			/* ���f�������_���[�̏������B*/
			if (!m_modelRender)
				m_modelRender = std::make_unique<ModelRender>();

			/* ���f���̏������ƃX�P�[�����O�B*/
			m_modelRender->Init(param.modelName.c_str());
			m_modelRender->SetScale(param.scale);
			m_modelRender->SetRotation(m_angle);
			m_modelRender->SetPosition(m_position);
			m_modelRender->Update();

			/* �����蔻��I�u�W�F�N�g�̏������B*/
			if (m_bulletCollider == nullptr)
			{
				m_bulletCollider = NewGO<nsK2Engine::CollisionObject>(0, "BulletCollision");
				m_bulletCollider->CreateSphere(m_position, Quaternion::Identity, param.radius);
				m_bulletCollider->SetIsEnableAutoDelete(false);
			}
			else
				m_bulletCollider->SetPosition(m_position);

			/* �{�X�N���X��T���B*/
			m_boss = FindGO<Boss>("boss");
		}


		void IGunBullet::Update()
		{
			/* ���g�p�Ȃ珈�����X�L�b�v�B*/
			if (!m_isInUse)
				return;

			m_deltaTime = g_gameTime->GetFrameDeltaTime();
			m_previousPosition = m_position;

			m_position += m_velocity * m_deltaTime;
			m_currentLifeTime -= m_deltaTime;

			if (m_currentLifeTime <= 0.0f)
			{
				DeleteGO(this);
				return;
			}

			if (m_bulletCollider != nullptr)
				m_bulletCollider->SetPosition(m_position);

			if (m_modelRender)
			{
				m_modelRender->SetPosition(m_position);
				m_modelRender->SetRotation(m_angle);
				m_modelRender->Update();
			}

			if (m_currentLifeTime <= 0.0f)
			{
				/* �폜�B*/
				Deactivate();
				return;
			}
		}


		void IGunBullet::Render(RenderContext& rc)
		{
			if (!m_isInUse)
				return;

			if (m_modelRender) 
				m_modelRender->Draw(rc);
		}


		bool IGunBullet::CheckHitBoss()
		{
			if (m_bulletCollider == nullptr)
				return false;

			if (m_boss != nullptr && reinterpret_cast<uintptr_t>(m_boss))
			{
				auto applyDamageToBoss = [this]()
				{
						DamageProcessor::ApplyDamageToTarget(m_boss, static_cast<int>(m_param.damage));
				};

				if (m_bulletCollider->IsHit(m_boss->GetController()))
				{
					applyDamageToBoss();
					return true;
				}

				m_bossPosition = m_boss->GetPosition();
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
							applyDamageToBoss();
							return true;
						}
					}
				}

			}

			return false;
		}


		void IGunBullet::Deactivate()
		{
			m_isInUse = false;

			m_currentLifeTime = 0.0f;
			m_velocity = Vector3::Zero;

			m_position = Vector3(0.0f, -100000.0f, 0.0f);
			m_previousPosition = m_position;

			if (m_bulletCollider != nullptr)
				m_bulletCollider->SetPosition(m_position);
		}
	}
}