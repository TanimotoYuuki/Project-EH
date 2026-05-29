#include "stdafx.h"
#include "MagicProjectotile.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Boss.h"

#include "Src/Effect/EffectList.h"

namespace nsApp
{
	namespace nsActor
	{
		MagicProjectotile::~MagicProjectotile()
		{
			if (m_magicCollider != nullptr)
				DeleteGO(m_magicCollider);
		}


		void MagicProjectotile::Initialize(MagicType type, const Vector3& spawnPosition, const Vector3& forwardDirection, const MagicParameter& param)
		{
			m_isInUse = true;
			m_target = nullptr;
			m_magicType = type;
			m_position = spawnPosition;
			m_previousPosition = spawnPosition;


			/* 寿命と速度を初期化（追尾計算で使うため保存）*/
			m_currentLifeTime = param.lifeTime;
			m_moveSpeed = param.speedPerSecond;
			m_velocity = forwardDirection * m_moveSpeed;

			/* モデルの大きさを初期化 */
			m_scale = param.scale;

			/* モデルの回転方向を初期化 */
			m_direction.SetRotation(Vector3::Front, forwardDirection);
			m_angle = m_direction * param.angle;

			/* ダメージ量を初期化 */
			SetDamage(param.damage);

			/* 各ミサイルモデルのパスを初期化 */
			if (!m_isModelInitialized)
			{
				m_missileModel = std::make_unique<ModelRender>();
				m_missileModel->Init(param.modelPath.c_str());
				m_isModelInitialized = true;
			}

			if (m_missileModel != nullptr)
			{
				m_missileModel->SetScale(m_scale);
				m_missileModel->SetRotation(m_angle);
				m_missileModel->SetPosition(m_position);
				m_missileModel->Update();
			}

			/* ミサイルの当たり判定を初期化 */
			if (m_magicCollider == nullptr)
			{
				m_magicCollider = NewGO<nsK2Engine::CollisionObject>(0, "MagicCollision");
				m_magicCollider->CreateSphere(m_position, m_angle, param.radius);
				m_magicCollider->SetIsEnableAutoDelete(false);
			}
			else
				m_magicCollider->SetPosition(m_position);
		}


		void MagicProjectotile::Update()
		{
			if (!m_isInUse)
				return;

			m_previousPosition = m_position;

			/* フレーム間の時間を取得 */
			float deltaTime = g_gameTime->GetFrameDeltaTime();

			/* 位置を更新する（超高速にならないよう deltaTime を掛ける！）*/
			m_position += m_velocity * deltaTime;

			/* 寿命を秒数で減らして消滅処理 */
			m_currentLifeTime -= deltaTime;
			if (m_currentLifeTime <= 0.0f)
			{
				Deactivate();
				return;
			}

			/* 連打魔法の場合、発動 */
			if (m_magicType == MagicType::enRushMagic)
				TargetMoving();

			/* 当たり判定の追従 */
			if (m_magicCollider != nullptr)
				m_magicCollider->SetPosition(m_position);

			if (m_missileModel != nullptr)
			{
				m_missileModel->SetRotation(m_angle);
				m_missileModel->SetScale(m_scale);
				m_missileModel->SetPosition(m_position);
				m_missileModel->Update();
			}
			if (CheckHitBoss())
			{
				Deactivate();
				return;
			}
		}


		void MagicProjectotile::Render(RenderContext& rc)
		{
			if (!m_isInUse)
				return;

			if (m_missileModel != nullptr)
				m_missileModel->Draw(rc);
		}


		void MagicProjectotile::TargetMoving()
		{
			float deltaTime = g_gameTime->GetFrameDeltaTime();

			if (m_scale.x < 1.5f)
				m_scale += Vector3::One * (0.02f * 60.0f * deltaTime);

			if (m_target != nullptr)
			{
				m_targetPosition = m_target->GetPosition();
				m_targetPosition.y += 10.0f;

				m_toTargetVector = m_targetPosition - m_position;

				if (m_toTargetVector.LengthSq() > 0.001f)
				{
					m_toTargetVector.Normalize();
					m_currentDirection = m_velocity;

					if (m_currentDirection.Length() > 0.001f)
					{
						m_currentDirection.Normalize();

						m_newPosition.Lerp(0.08f * 60.0f * deltaTime, m_currentDirection, m_toTargetVector);
						m_newPosition.Normalize();
						m_velocity = m_newPosition * m_moveSpeed; // 秒間速度を掛け直す
						m_direction.SetRotation(Vector3::Front, m_newPosition);
					}
				}
			}
		}


		bool MagicProjectotile::CheckHitBoss()
		{
			if (m_magicCollider == nullptr)
				return false;

			auto*boss = FindGO<nsActor::Boss>("boss");
			if (boss == nullptr || reinterpret_cast<uintptr_t>(boss) == 0xFFFFFFFFFFFFFFFF)
				return false;

			auto applyDamageToBoss = [this, boss]()
				{
					DamageProcessor::ApplyDamageToTarget(boss, static_cast<int>(m_damage));
				};

			if (m_magicCollider->IsHit(boss->GetController()))
			{
				applyDamageToBoss();
				PlayHitEffect(m_position);
				return true;
			}

			m_bossPosition = boss->GetPosition();
			m_bossPosition.y += 50.0f;

			m_missileTrajectory = m_position - m_previousPosition;
			m_vectorToBossTarget = m_bossPosition - m_previousPosition;
			m_trajectoryLengthSquared = m_missileTrajectory.LengthSq();

			if (m_trajectoryLengthSquared > 0.0f)
			{
				m_closestPointRatio = m_vectorToBossTarget.Dot(m_missileTrajectory) / m_trajectoryLengthSquared;

				if (m_closestPointRatio >= 0.0f && m_closestPointRatio <= 1.0f)
				{
					m_closestPointOnTrajectory = m_previousPosition + (m_missileTrajectory * m_closestPointRatio);
					m_distanceToBoss = (m_bossPosition - m_closestPointOnTrajectory).Length();

					if (m_distanceToBoss < 150.0f)
					{
						applyDamageToBoss();
						PlayHitEffect(m_closestPointOnTrajectory);
						return true;
					}
				}
			}

			return false;
		}


		void MagicProjectotile::Deactivate()
		{
			m_isInUse = false;

			m_currentLifeTime = 0.0f;
			m_velocity = Vector3::Zero;
			m_target = nullptr;
			m_effectList = nullptr;

			m_position = Vector3(0.0f, -100000.0f, 0.0f);
			m_previousPosition = m_position;

			if (m_magicCollider != nullptr)
				m_magicCollider->SetPosition(m_position);
		}


		void MagicProjectotile::PlayHitEffect(const Vector3& position)
		{
			if (m_effectList == nullptr)
				return;

			/* エフェクトを再生する座標を設定。*/
			m_effectPosition = position;
			m_effectPosition.y += 10.0f;

			/* エフェクトを設定。*/
			m_effectList->PlayEffect(nsEffect::Hit,
				m_effectPosition,
				Quaternion::Identity,
				Vector3::One * 8.0f);
		}
	}
}