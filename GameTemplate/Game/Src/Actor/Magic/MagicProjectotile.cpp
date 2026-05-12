#include "stdafx.h"
#include "MagicProjectotile.h"
#include "Boss.h"
#include <unordered_map>

namespace nsApp
{
	namespace nsActor
	{
		namespace
		{
			/* モデル名を取得。*/
			inline const std::string SearchMissileModelPath(const std::string& modelName)
			{
				return "Assets/modelData/Character/Weapon/" + modelName + ".tkm";
			}

			/* 角度計算。*/
			Quaternion MakeAngle(float degX, float degY, float degZ)
			{
				Quaternion angleX, angleY, angleZ;
				angleX.SetRotationDegX(degX);
				angleY.SetRotationDegY(degY);
				angleZ.SetRotationDegZ(degZ);
				return angleX * angleY * angleZ;
			}

			const auto NORMAL_MAGIC_ANGLE = MakeAngle(90.0f, 0.0f, 0.0f); //! 通常魔法の角度。
			const auto MISSILE_SCAL = Vector3::One * 0.5f;                //! ミサイルの大きさ。

			/*
             * @struct
             * @brief
             * 魔法を構成する要素をまとめた構造体。
             */
			struct MagicParam
			{
				float speed = 0.0f;					      //! 魔法の射出速度。
				float damage = 0.0f;					  //! 魔法のダメージ量。
				std::string modelPath;				      //! モデル名。
				float hitRadius = 0.0f;					  //! 魔法の当たり判定の半径。
				Vector3 scale = Vector3::One;			  //! 魔法の大きさ。
				Quaternion angle = Quaternion::Identity;  //! 射出オブジェクトの角度。
			};

			/*
			 * @brief 魔法の種類とそのパラメータを紐づけるテーブル。
			 * @param: 第1引数: 魔法の種類。
			 * @param: 第2引数: 魔法のパラメータ(MagicParam参照)。
			 */
			const std::unordered_map<MagicType, MagicParam> MAGIC_PARAM_TABLE =
			{
				{ MagicType::enNormalMagic, MagicParam{2.0f, 10.0f, SearchMissileModelPath("NormalAttackMissile"), 10.0f, MISSILE_SCAL, NORMAL_MAGIC_ANGLE} },
				{ MagicType::enChargeMagic, MagicParam{2.0f, 10.0f, SearchMissileModelPath("ChargeAttackMissile"), 10.0f, MISSILE_SCAL, NORMAL_MAGIC_ANGLE} },
				{ MagicType::enRushMagic,   MagicParam{2.0f, 10.0f, SearchMissileModelPath("PressingAttackMissile"), 10.0f, MISSILE_SCAL, NORMAL_MAGIC_ANGLE} },
				{ MagicType::enAirMagic,    MagicParam{2.0f, 10.0f, SearchMissileModelPath("NormalAttackMissile"), 10.0f, MISSILE_SCAL, NORMAL_MAGIC_ANGLE} },
			};
		}


		MagicProjectotile::~MagicProjectotile()
		{
			if(m_magicCollider != nullptr)
				DeleteGO(m_magicCollider);
		}


		void MagicProjectotile::Initialize(MagicType type, const Vector3& spawnPosition, const Vector3& forwardDirection, const MagicParameter& param)
		{
			m_magicType = type;
			m_position = spawnPosition;
			
			/* 射出速度を初期化。*/
			m_velocity = forwardDirection * param.speedPerSecond;
			/* モデルの大きさを初期化。*/
			m_scale = param.scale;

			/* モデルの回転方向を初期化。*/
			m_direction.SetRotation(Vector3::Front, forwardDirection);
			m_angle = m_direction * param.angle;

			/* ダメージ量を初期化。*/
			SetDamage(param.damage);
			/* 各ミサイルモデルのパスを初期化。*/
			m_missileMddel.Init(param.modelPath.c_str());

			/* ミサイルの当たり判定を初期化。*/
			m_magicCollider = NewGO<nsK2Engine::CollisionObject>(0, "MagicCollision");
			m_magicCollider->CreateSphere(m_position, m_angle, param.hitRadius);
			m_magicCollider->SetIsEnableAutoDelete(false);
		}


		void MagicProjectotile::Update()
		{
			m_previousPosition = m_position;

			/* 位置を更新する。*/
			m_position += m_velocity;

			/* 消滅処理。*/
			m_lifeTimer++;
			if (m_lifeTimer > 60.0f)
			{
				DeleteGO(this);
				return;
			}

			/* 連打魔法の場合、発動。*/
			if (m_magicType == MagicType::enRushMagic)
				TargetMoving();

			/* モデルの各要素の更新。*/
			/* 角度。*/
			m_missileMddel.SetRotation(m_angle);
			/* 大きさ。*/
			m_missileMddel.SetScale(m_scale);
			/* 座標。*/
			m_missileMddel.SetPosition(m_position);
			/* 最終更新。*/
			m_missileMddel.Update();

			if(CheckHitBoss())
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
			/* 時間差でモデルを少しずつ大きくする。*/
			if (m_scale.x < 1.5f)
				m_scale += Vector3::One * 0.02f;

			/* 目標が設定されている場合。*/
			if (m_target != nullptr)
			{
				m_targetPosition = m_target->GetPosition();
				m_targetPosition.y += 10.0f;

				/* 自身と目標の距離を測る。*/
				m_toTargetVector = m_targetPosition - m_position;

				if (m_toTargetVector.LengthSq() > 0.001f)
				{
					m_toTargetVector.Normalize();

					/* 目標の方向を向く。*/
					m_currentDirection = m_velocity;
					m_moveSpeed = m_currentDirection.Length();

					if (m_moveSpeed > 0.001f)
					{
						m_newPosition.Lerp(0.08f, m_currentDirection, m_toTargetVector);
						m_newPosition.Normalize();

						m_velocity = m_newPosition * m_moveSpeed;

						m_direction.SetRotation(Vector3::Front, m_newPosition);
						m_angle = m_direction * NORMAL_MAGIC_ANGLE;
					}
				}
			}
		}	


		bool MagicProjectotile::CheckHitBoss()
		{
			if (m_magicCollider == nullptr)
				return false;


			auto boss = FindGO<nsActor::Boss>("boss");
			if (boss != nullptr && reinterpret_cast<uintptr_t>(boss) != 0xFFFFFFFFFFFFFFFF)
			{
				if (m_magicCollider->IsHit(boss->GetController())) {
					boss->ApplyDamage(static_cast<int>(m_damage));
					return true;
				}

				m_bossPosition = boss->GetPosition();
				m_bossPosition.y += 50.0f; // ボスの高さに合わせて中心を狙う

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