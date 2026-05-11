#pragma once
/**
 * @file   IGunBullet.h
 * @brief  弾丸の親クラス。
 * @author Yamaguchi Hayato
 * @date 2026/04/30
 */

#include "Src/Actor/Actor.h"

namespace nsApp
{
	namespace nsActor
	{
		class IGunBullet : public Actor
		{
		public:
			/* コンストラクタとデストラクタ。*/
			IGunBullet() = default;
			virtual ~IGunBullet();


		public:
			/**
			 * @brief 弾丸の初期化処理。
			 * @param spawnPosition 弾丸のスポーン位置。
			 * @param shotDirection 弾丸の発射方向。
			 * @param speed 弾丸の速度。
			 * @param lifeTime 弾丸の寿命。
			 */
			virtual void InitializeBullet(const Vector3& spawnPosition, const Vector3& shotDirection, float speed, float lifeTime)
			{
				m_position = spawnPosition;
				m_direction = shotDirection;
				m_speed = speed;
				m_lifeTime = lifeTime;
			}


		    /**
		     * @brief 初期化処理。
		     */
			virtual bool Start() override 
			{
				return false; 
			};


		    /**
		     * @brief 更新処理。 
		     */
			virtual void Update() override = 0; 


		protected:
			/**
			 * @brief コリジョンを弾丸に追従。
			 * @param radius コリジョンの半径。
			 */
			void InitCollision(Quaternion angle, float radius);


			/**
			 * @brief コリジョンの座標を弾丸の座標にあわせる。 
			 */
			inline void UpdateBulletCollisionPosition()
			{
				if (m_bulletCollider) 
					m_bulletCollider->SetPosition(m_position);
			}

			/**
			 * @brief 弾丸の寿命時間を管理。
			 */
			bool CheckLifeTime()
			{
				m_lifeTime -= g_gameTime->GetFrameDeltaTime();
				if (m_lifeTime <= 0)
					return true;

				return false;
			}

			/**
			 * @brief 対象との衝突判定を検知。。 
			 */
			bool CheckHitBoss();


		protected:
			CollisionObject* m_bulletCollider = nullptr;		//! 弾丸の当たり判定オブジェクト。


		protected:
			Vector3 m_position = Vector3::Zero;                 //! 弾丸の現在の位置。
			Vector3 m_previousPosition = Vector3::Zero;         //! 弾丸の1フレーム前の位置（すり抜け防止の計算用）。
			Vector3 m_direction = Vector3::Zero;                //! 弾丸の発射方向。
			Vector3 m_bossPosition = Vector3::Zero;             //! ボスの中心位置。
			Vector3 m_bulletTrajectory = Vector3::Zero;         //! 弾丸が1フレームで移動した軌跡（線）のベクトル。
			Vector3 m_vectorToBossTarget = Vector3::Zero;       //! 弾丸の過去の位置から、ボスに向かうベクトル。
			Vector3 m_closestPointOnTrajectory = Vector3::Zero; //! 弾の軌跡上で、最もボスに近づいた瞬間の座標。

			float m_speed = 0.0f;                               //! 弾丸の速度。
			float m_lifeTime = 0.0f;                            //! 弾丸の寿命。
			float m_distanceToBoss = 0.0f;                      //! 弾の軌跡と、ボスの実際の距離。
			float m_trajectoryLengthSquared = 0.0f;             //! 弾の軌跡の長さの2乗（計算用）。
			float m_closestPointRatio = 0.0f;                   //! 軌跡の長さに対して、ボスに一番近い点が「何割」の位置にあるか（0.0なら過去位置、1.0なら今の位置）。
		};
	}
}
