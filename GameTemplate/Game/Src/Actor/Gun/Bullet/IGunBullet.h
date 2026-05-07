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
			virtual ~IGunBullet() = default;


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
			virtual bool Start() override { return false; };


		    /**
		     * @brief 更新処理。 
		     */
			virtual void Update() override = 0; 


		protected:
			/*
			 * @brief 弾丸の寿命時間を管理。
			 */
			bool CheckLifeTime()
			{
				m_lifeTime -= g_gameTime->GetFrameDeltaTime();
				if (m_lifeTime <= 0)
					return true;

				return false;
			}


		protected:
			Vector3 m_position = Vector3::Zero;      //! 弾丸の位置。
			Vector3 m_direction = Vector3::Zero;     //! 弾丸の発射方向。

			float m_speed = 0.0f;                    //! 弾丸の速度。
			float m_lifeTime = 0;                    //! 弾丸の寿命。
		};
	}
}
