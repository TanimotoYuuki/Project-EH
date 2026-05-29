#pragma once
/**
 * @file   IGunBullet.h
 * @brief  すべての弾丸の処理を統一したクラス。
 * @author Yamaguchi Hayato
 */

#include "Src/Actor/Actor.h"
#include "Src/Actor/Gun/Parameter/BulletParameter.h"

namespace nsApp
{
	namespace nsActor
	{
		class IGunBullet : public Actor
		{
		public:
			IGunBullet() = default;
			virtual ~IGunBullet();

			/**
			 * @brief 弾丸の初期化。パラメータを丸ごと受け取る。
			 * @param param 弾丸のパラメータ。BulletParameter構造体でまとめて管理。
			 * @param spawnPosition 弾丸の生成位置。プレイヤーの位置や向きから計算される。
			 */
			void Initialize(const BulletParameter& param, const Vector3& spawnPosition, const Vector3& forwardDirection);


		public:
			/* ライフサイクル。*/
			bool Start() override
			{
				return true; 
			}
			void Update() override;
			void Render(RenderContext& rc) override;


		private:
			/**
			 * @brief　弾丸がボスにヒットしたかを判定する関数。直接の球体判定と、すり抜け防止の線分判定の両方を行う。
			 * @return 衝突判定を返す。
			 */
			bool CheckHitBoss();


			void TargetMoving() {};


		private:
			BulletParameter m_param;									//! 弾丸のパラメータ。 
			nsK2Engine::CollisionObject* m_bulletCollider = nullptr;    //! 弾丸の当たり判定オブジェクト。
			std::unique_ptr<ModelRender> m_modelRender;					//! 弾丸のモデルレンダラー。


		private:
			Vector3 m_position = Vector3::Zero;							//! 弾丸の現在位置。
			Vector3 m_previousPosition = Vector3::Zero;					//! 弾丸の前フレームの位置。軌道計算に使用。
			Vector3 m_velocity = Vector3::Zero;							//! 弾丸の速度ベクトル。
			Vector3 m_bossPosition = Vector3::Zero;						//! ボスの位置。ヒット判定や軌道計算に使用。
			Vector3 m_bulletTrajectory = Vector3::Zero;					//! 弾丸の軌道ベクトル。前フレームからの移動量。
			Vector3 m_vectorToBossTarget = Vector3::Zero;				//! ボスへのベクトル。前フレームからボスへの距離を表す。
			Vector3 m_closestPointOnTrajectory = Vector3::Zero;			//! 弾丸の軌道上の最も近い点。ボスへの距離計算に使用。

			Quaternion m_angle = Quaternion::Identity;					//! 弾丸の回転角。モデルの向きに合わせて設定。
			Quaternion m_direction = Quaternion::Identity;				//! 弾丸の向き。前方向ベクトルから計算される。

			float m_currentLifeTime = 0.0f;								//! 弾丸の現在の寿命。初期化時にパラメータから設定され、時間経過で減少する。
			float m_speedPerSecond = 0.0f;								//! 弾丸の速度。初期化時にパラメータから設定され、前方向ベクトルに乗算される。
			float m_distanceToBoss = 0.0f;								//! ボスへの距離。ヒット判定に使用。
			float m_trajectoryLengthSquared = 0.0f;						//! 弾丸の軌道の長さの二乗。軌道上の最も近い点を計算するために使用。
			float m_closestPointRatio = 0.0f;							//! 弾丸の軌道上の最も近い点の位置を表す比率。0.0fは前フレームの位置、1.0fは現在の位置を表す。
			float m_deltaTime = 0.0f;									//! フレームごとの時間差。Update関数内で計算され、軌道計算や寿命減少に使用される。
		};
	}
}