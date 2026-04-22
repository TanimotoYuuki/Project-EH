#include "stdafx.h"
#include "MagicProjectotile.h"
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


		void MagicProjectotile::Initialize(MagicType type, const Vector3& spawnPosition, const Vector3& forwardDirection)
		{
			m_magicType = type;
			m_position = spawnPosition;
			m_lifeTimer = 0;

			/* テーブルから条件にあった魔法を取り出す。*/
			const auto& param = MAGIC_PARAM_TABLE.at(type);

			/* 射出速度を初期化。*/
			m_velocity = forwardDirection * param.speed;
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
			m_hitDetection.Init(param.hitRadius);
		}


		void MagicProjectotile::Update()
		{
			/* 位置を更新する。*/
			m_position += m_velocity;

			/* 消滅処理。*/
			m_lifeTimer++;
			if (m_lifeTimer > 60.0f)
			{
				DeleteGO(this);
				return;
			}

			/* モデルの各要素の更新。*/
			/* 角度。*/
			m_missileMddel.SetRotation(m_angle);
			/* 大きさ。*/
			m_missileMddel.SetScale(m_scale);
			/* 座標。*/
			m_missileMddel.SetPosition(m_position);
			/* 最終更新。*/
			m_missileMddel.Update();
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
				/* 目標の中心位置を狙う。*/
				m_targetPosition = m_target->GetPosition();
				m_targetPosition.y += 10.0f;

				/* 自身と目標の距離を測る。*/
				m_toTargetVector = m_targetPosition - m_position;
				m_toTargetVector.Normalize();

				/* 目標の方向を向く。*/
				m_toTargetVector = m_velocity;
				m_moveSpeed = m_currentDirection.Length();
				m_currentDirection.Normalize();

				/* 現在の進行方向を目標の方向に滑らかに向ける。*/
				m_newPosition.Lerp(0.0f, m_currentDirection, m_targetPosition);
				m_newPosition.Normalize();

				/* 新しい進行方向と速度を適応。*/
				m_velocity = m_newPosition * m_moveSpeed;

				/* モデルの角度も計算する。*/
				m_direction.SetRotation(Vector3::Front, m_newPosition);
				m_angle = m_direction * NORMAL_MAGIC_ANGLE;
					
			}






			if (m_magicType == MagicType::enRushMagic)
			{
				/* */
			}
		}
	}
}