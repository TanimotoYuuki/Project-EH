#pragma once
/**
 * @file   MagicProjectotile.h
 * @brief  攻撃の際に飛ばすオブジェクトを管理するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/03/23
 */

#include "Src/Actor/Character/Common/WeaponHitDetection.h"
#include "Src/Actor/Magic/Parameter/MagicParameter.h"

namespace nsApp
{
	namespace nsActor
	{
		enum class MagicType : uint8_t
		{
			enNormalMagic,  //! 通常魔法。
			enRushMagic,    //! 連打魔法。
			enAirMagic,     //! 空中魔法。
			enChargeMagic,  //! チャージ魔法。
			enNone,         //! 魔法なし。
		};


		class MagicProjectotile : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			MagicProjectotile() = default;
			virtual ~MagicProjectotile();


		public:
			/* 
			 * @brief 初期化処理。
			 * @param type:             魔法の種類。
			 * @param spawnPosition: 　 魔法の出現位置。
			 * @param forwardDirection: 前方向のベクトル。
			 * @param param:            魔法のパラメータ。
			 */
			void Initialize(MagicType type, const Vector3& spawnPosition, const Vector3& forwardDirection, const MagicParameter& param);


			/**
			 * @brief 更新処理。   
			 */
			void Update();

			/**
			 * @brief 描画処理。
			 * @detail ここでは攻撃時に射出するミサイルオブジェクトの描画を行う。
			 */
			void Render(RenderContext& rc);


		private:
			/**
			 * @brief ターゲットに向かって移動する処理。
			 */
			void TargetMoving();

			/**
			 * @brief 衝突判定処理。 
			 */
			bool CheckHitBoss();


		/* セッター。*/
		public:
			/* 
			 * @brief ダメージ数を設定。
			 * @param damage 設定するダメージ数。
			 */
			inline void SetDamage(float damage)
			{
				m_damage = damage;
			}

			/* 
			 * @brief 目標を設定。
			 * @param target 設定する目標。
			 */
			inline void SetTarget(nsActor::ICharacter* target)
			{
				m_target = target;
			}


		/* ゲッター。*/
		public:
			/* 魔法の種類を取得。*/
			inline MagicType GetMagicType() const
			{
				return m_magicType;
			}

			/* モデル名を取得。*/
			inline const std::string SearchMissileModelPath(const std::string& modelName)
			{
				return "Assets/modelData/Character/Weapon/" + modelName + ".tkm";
			}

			/* 
			 * @brief 角度計算。
			 * @param degX X軸の回転角度。
			 * @param degY Y軸の回転角度。
			 * @param degZ Z軸の回転角度。
			 */
			Quaternion MakeAngle(float degX, float degY, float degZ)
			{
				m_angleX.SetRotationDegX(degX);
				m_angleY.SetRotationDegY(degY);
				m_angleZ.SetRotationDegZ(degZ);
				return m_angleZ * m_angleY * m_angleZ;
			}


		private:
			nsActor::ICharacter* m_target = nullptr;			 //! 目標。
			nsK2Engine::CollisionObject* m_magicCollider;        //! 魔法の当たり判定を管理するクラス。

			
		private:
			MagicType m_magicType = MagicType::enNone;           //! 魔法の種類。


			Vector3 m_position = Vector3::Zero;                  //! 魔法の位置。
			Vector3 m_velocity = Vector3::Zero;					 //! 魔法の速度。
			Vector3 m_forwardDirection = Vector3::Zero;			 //! 魔法の前方向ベクトル。
			Vector3 m_scale = Vector3::One;						 //! 魔法のスケール。
			Vector3 m_targetPosition = Vector3::Zero;		  	 //! 目標位置。
			Vector3 m_toTargetVector = Vector3::Zero;			 //! 目標と自身の距離ベクトル。
			Vector3 m_currentDirection = Vector3::Zero;			 //! 現在の移動方向。
			Vector3 m_newPosition = Vector3::Zero;               //! 新しい位置。
			Vector3 m_previousPosition = Vector3::Zero;          //! 前フレームの位置。
			Vector3 m_bossPosition = Vector3::Zero;				 //! ボスの位置。
			Vector3 m_missileTrajectory = Vector3::Zero;	     //! ミサイルの軌道。
			Vector3 m_vectorToBossTarget = Vector3::Zero;		 //! ボスの目標位置へのベクトル。
			Vector3 m_closestPointOnTrajectory = Vector3::Zero;  //! 軌道上の最も近い点。

			Quaternion m_angle = Quaternion::Identity;			 //! 魔法の回転角。
			Quaternion m_direction = Quaternion::Identity;       //! 角度。
			Quaternion m_angleX = Quaternion::Identity;			 //! X軸の回転角。
			Quaternion m_angleY = Quaternion::Identity;          //! Y軸の回転角。
			Quaternion m_angleZ = Quaternion::Identity;          //! Z軸の回転角。


			ModelRender m_missileMddel;							 //! ミサイルを描画するレンダー。

			float m_currentLifeTime = 0.0f;                      //! 秒数管理用の寿命			
			float m_damage = 0.0f;                               //! 魔法のダメージ量。
			float m_moveSpeed = 0.0f;                            //! 移動速度。
			float m_trajectoryLengthSquared = 0.0f;              //! 軌道の長さの二乗。
			float m_closestPointRatio = 0.0f;                    //! 最も近い点の比率。
			float m_distanceToBoss = 0.0f;						 //! ボスまでの距離。
		};
	}
}