#include "stdafx.h"
#include "MagicProjectotile.h"
#include "Boss.h"

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
			m_magicType = type;
			m_position = spawnPosition;

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
			m_missileMddel.Init(param.modelPath.c_str());

			/* ミサイルの当たり判定を初期化 */
			m_magicCollider = NewGO<nsK2Engine::CollisionObject>(0, "MagicCollision");
			m_magicCollider->CreateSphere(m_position, m_angle, param.radius);
			m_magicCollider->SetIsEnableAutoDelete(false);
		}

		void MagicProjectotile::Update()
		{
			m_previousPosition = m_position;

			/* フレーム間の時間を取得 */
			float deltaTime = g_gameTime->GetFrameDeltaTime();

			/* 位置を更新する（超高速にならないよう deltaTime を掛ける！）*/
			m_position += m_velocity * deltaTime;

			/* 寿命を秒数で減らして消滅処理 */
			m_currentLifeTime -= deltaTime;
			if (m_currentLifeTime <= 0.0f)
			{
				DeleteGO(this);
				return;
			}

			/* 連打魔法の場合、発動 */
			if (m_magicType == MagicType::enRushMagic)
				TargetMoving();

			/* 当たり判定の追従 */
			if (m_magicCollider != nullptr)
				m_magicCollider->SetPosition(m_position);

			/* モデルの各要素の更新 */
			m_missileMddel.SetRotation(m_angle);
			m_missileMddel.SetScale(m_scale);
			m_missileMddel.SetPosition(m_position); // ★これが抜けていたので描画がバグっていました
			m_missileMddel.Update();

			if (CheckHitBoss())
			{
				DeleteGO(this);
				return;
			}
		}

		void MagicProjectotile::Render(RenderContext& rc)
		{
			m_missileMddel.Draw(rc);
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
			if (m_magicCollider == nullptr) return false;

			auto boss = FindGO<nsActor::Boss>("boss"); // 銃と表記を統一（大文字のBoss）
			if (boss != nullptr && reinterpret_cast<uintptr_t>(boss) != 0xFFFFFFFFFFFFFFFF)
			{
				if (m_magicCollider->IsHit(boss->GetController())) {
					boss->ApplyDamage(static_cast<int>(m_damage));
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

						if (m_distanceToBoss < 150.0f) {
							boss->ApplyDamage(static_cast<int>(m_damage));
							return true;
						}
					}
				}
			}
			return false;
		}
	}
}