#pragma once
/**
 * @file   PlayerAirAttackState.h
 * @brief  プレイヤーの空中攻撃ステート。
 * @author Yamaguchi Hayato
 * @date   2026/06/15
 */

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"

namespace nsApp
{
	namespace nsActor
	{
		class Player;
		class MagicProjectotile;
		class AirBullet;
	}

	namespace nsState
	{
		class PlayerJumpState;

		class PlayerAirAttackState : public PlayerAttackBaseState
		{
		public:
			PlayerAirAttackState() = default;
			virtual ~PlayerAirAttackState() = default;


		protected:
			// --- Template Method  ---
			void PlayAttackAnimation() override;
			void OnEnterAttack() override;
			bool OnUpdateAttack() override;
			void OnAttackTick() override;
			void OnExitAttack() override;


		private:
			/**
			 * @brief 空中での移動処理。
			 * @param deltaTime 1フレームの経過時間。
			 */
			void UpdateAirMovement(float deltaTime);

			/**
			 * @brief 着地判定。
			 */
			bool CheckLanding();

			/**
			 * @brief エフェクトの生成。
			 * @detail 着地時の衝撃波エフェクトを生成する。
			 */
			void CreateShockWaveEffect();

			/**
			 * @brief アニメーションの終了判定。
			 * @return アニメーションが終了している場合はtrue、そうでない場合はfalse。
			 */
			bool CheckAnimationEndTransition();

			/**
			 * @brief ミサイルをばら撒くように計算。
			 * @param angle ミサイルをばら撒いていく角度。
			 */
			Vector3 ComputeMissile(float angle);

			/**
			 * @brief ミサイルを生成。
			 */
			void SpawnMissile();

			/**
			 * @brief 弾丸を発射する。
			 */
			void FireAirBullet();


		public:
			/**
			 * @brief 落下速度を設定。
			 * @param velocity 落下速度の値。
			 */
			inline void SetFallVelocity(float velocity)
			{
				m_fallVelocity = velocity;
			}

			/**
			 * @brief 重力の強さを設定。
			 * @param gravity 重力の強さ。
			 */
			inline void SetGravity(float gravity)
			{
				m_gravity -= gravity;
			}

			/**
			 * @brief 空中での移動速度を設定。
			 * @param moveSpeed 空中での移動速度のベクトル。
			 */
			inline void SetMoveSpeed(const Vector3 moveSpeed)
			{
				m_moveSpeed = moveSpeed;
			}

			/**
			 * @brief 空中での移動速度を設定。
			 * @param speed 速度。
			 */
			inline void SetAirMoveSpeed(float speed)
			{
				m_airMoveSpeed = speed;
			}


		private:
			nsActor::MagicProjectotile* m_airMissile = nullptr;  //! 空中攻撃で生成するミサイルのポインタ
			nsActor::AirBullet* m_bulletR = nullptr;             //! 右手から発射される弾のポインタ
			nsActor::AirBullet* m_bulletL = nullptr;             //! 左手から発射される弾のポインタ
			nsState::PlayerJumpState* m_jumpState = nullptr;     //! 遷移先のジャンプステートポインタ


		private:
			float m_airMoveSpeed = 0.0f;                         //! 空中移動速度
			float m_fallVelocity = 0.0f;                         //! 落下速度
			float m_gravity = 0.0f;                              //! 重力の強さ

			bool m_isLanding = false;                            //! 着地したかどうかのフラグ
			bool m_isSpawningMissile = false;                    //! ミサイルを生成しているかどうかのフラグ
			bool m_isHovering = false;                           //! 滞空状態かどうかのフラグ

			Vector3 m_moveSpeed = Vector3::Zero;                 //! 空中での移動速度
			Vector3 m_currentAirMoveSpeed = Vector3::Zero;       //! 空中での現在の移動速度
			Vector3 m_landingPosition = Vector3::Zero;           //! 着地した位置
			Vector3 m_shootDirection = Vector3::Zero;            //! ミサイルを撃つ方向
			Vector3 m_missileShootDir = Vector3::Zero;           //! ミサイルを撃つ方向
			Vector3 m_missileSpawnPos = Vector3::Zero;           //! ミサイルの生成位置
			Vector3 m_gunShootDir = Vector3::Zero;               //! 銃弾を撃つ方向
			Vector3 m_gunSpawnPos = Vector3::Zero;               //! 銃弾の生成位置
			Vector3 m_spawnPosR = Vector3::Zero;                 //! 右手の銃口位置
			Vector3 m_spawnPosL = Vector3::Zero;                 //! 左手の銃口位置
			Vector3 m_recoilVector = Vector3::Zero;              //! 反動ベクトル
			Vector3 m_recoil = Vector3::Zero;                    //! 反動計算用の一時ベクトル

			Quaternion m_angleY = Quaternion::Identity;          //! Y軸の回転角度
			Quaternion m_baseRot = Quaternion::Identity;         //! 武器の基本角度
			Quaternion m_aimDown = Quaternion::Identity;         //! 武器を下に向けるための角度
		};
	}
}