#include "stdafx.h"
#include "PlayerAirAttackState.h"

#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"
#include "Src/Debug/Sandbag.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Actor/Magic/MagicProjectotile.h" 
#include "Src/Actor/Gun/Register/BulletModelRegister.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace
{
	const auto MOVE_FRAME_TIME = 1.0f / 60.0f;             //! 1フレームあたりの固定時間
	const auto AIR_MOVE_SPEED = 120.0f;                    //! 空中での前後左右のスピード
	const auto AIR_ATTACK_TIMER_INIT = 0;                  //! 空中攻撃のタイマーの初期値
	const auto AIR_ATTACK_DURATION = 25;                   //! 魔法攻撃の着地判定時間
	const auto FALL_ACCELERATION = 30.0f;                  //! 落下加速度
	const auto MAX_FALL_VELOCITY = -1200.0f;               //! 落下速度の最大値

	const auto RECOIL_POWER_BACK = -80.0f;                 //! 射撃時の後方ノックバック力
	const auto RECOIL_POWER_UP = 15.0f;                    //! 射撃時の上方浮遊力
	const auto RECOIL_DECAY_RATE = 0.85f;                  //! 反動の減衰率

	const auto BULLET_SPEED = 50.0f;                       //! 弾の速度
	const auto BULLET_LIFE_TIME = 40;                      //! 弾の生存時間
	const auto BULLET_SHOOT_DIR_DOWN_OFFSET = 0.5f;        //! 弾の発射方向の下方補正

	const auto WEAPON_ANGLE_BASE_Z = -90.0f;               //! 双銃の基本Z軸角度
	const auto WEAPON_AIM_DOWN_X = 90.0f;                  //! 銃口を下に向けるX軸角度

	const auto FIRE_FRAME_FIRST = 5;                       //! 1発目の発射フレーム
	const auto FIRE_FRAME_SECOND = 15;                     //! 2発目の発射フレーム
	const auto HOVERING_END_FRAME = 30;                    //! 滞空終了フレーム
	const auto WAND_SPAWN_FRAME = 10;                      //! 杖ミサイル生成フレーム

	const auto SHOCKWAVE_OFFSET_X = 50.0f;                 //! 衝撃波のX軸オフセット
	const auto SHOCKWAVE_SCALE = 2.0f;                     //! 衝撃波のスケール
	const auto MISSILE_SPAWN_OFFSET_Y = 10.0f;             //! ミサイル生成のY軸オフセット

	const float MISSILE_ANGLE_LIST[] = { -45.0f, -22.5f, 0.0f, 22.5f, 45.0f }; //! ミサイル角度
}

#define GET_PLAYER_CHARACON m_player->GetCharacterController()

namespace nsApp
{
	namespace nsState
	{
		void PlayerAirAttackState::Enter()
		{
			m_player = static_cast<nsActor::Player*>(m_owner);
			m_currentAttackType = AttackType::AirAttack;

			SetAttackTimer(AIR_ATTACK_TIMER_INIT);
			m_recoilVector = Vector3::Zero;

			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				m_player->LoadSubWeapon(CharacterModelType::Weapon_TwinGun);

				m_baseRot = Quaternion::Identity;
				m_baseRot.SetRotationDegZ(WEAPON_ANGLE_BASE_Z);

				m_aimDown = Quaternion::Identity;
				m_aimDown.SetRotationDegX(WEAPON_AIM_DOWN_X);

				m_player->SetWeaponRotationByQuaternion(m_baseRot * m_aimDown);
			}
			else
			{
				m_player->PlayWeaponAnimation(AttackType::AirAttack);
			}

			m_player->GetWeaponHitDetection().Enable();
		}

		void PlayerAirAttackState::Update()
		{
			m_attackTimer++;

			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				if (m_attackTimer == FIRE_FRAME_FIRST || m_attackTimer == FIRE_FRAME_SECOND)
				{
					FireAirBullet();
				}
			}
			else if (m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				if (m_attackTimer == WAND_SPAWN_FRAME && !m_isSpawningMissile)
				{
					SpawnMissile();
					m_isSpawningMissile = true;
				}
			}

			if (!m_isLanding)
			{
				UpdateAirMovement(MOVE_FRAME_TIME);
				CheckLanding();
			}
			else
			{
				if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				{
					m_player->SetPostureOffset(Quaternion::Identity);
					m_stateMachine->ChangeState(new PlayerIdleState());
					return;
				}

				else
				{
					if (!m_player->IsPlayAnimation())
					{
						m_stateMachine->ChangeState(new PlayerIdleState());
						return;
					}
				}
			}
		}

		void PlayerAirAttackState::Exit()
		{
			if (m_player && m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				m_baseRot = Quaternion::Identity;
				m_baseRot.SetRotationDegZ(WEAPON_ANGLE_BASE_Z);
				m_player->SetWeaponRotationByQuaternion(m_baseRot);
			}
			PlayerAttackBaseState::Exit();
		}

		void PlayerAirAttackState::UpdateAirMovement(float deltaTime)
		{
			m_currentAirMoveSpeed = Vector3::Zero;
			const auto& inputClass = m_player->GetInputClass();

			if (inputClass.IsMove())
			{
				m_currentAirMoveSpeed.x = inputClass.GetMoveVector().x * AIR_MOVE_SPEED;
				m_currentAirMoveSpeed.z = inputClass.GetMoveVector().z * AIR_MOVE_SPEED;
			}

			m_currentAirMoveSpeed += m_recoilVector;

			m_recoilVector.x *= RECOIL_DECAY_RATE;
			m_recoilVector.z *= RECOIL_DECAY_RATE;

			m_isHovering = (m_player->GetCurrentWeapon() == WeaponType::TwinGun && m_attackTimer <= HOVERING_END_FRAME);

			if (m_isHovering)
			{
				m_fallVelocity = m_recoilVector.y;
				m_recoilVector.y *= RECOIL_DECAY_RATE;
			}
			else
			{
				m_fallVelocity -= FALL_ACCELERATION;
			}

			if (m_fallVelocity < MAX_FALL_VELOCITY)
			{
				m_fallVelocity = MAX_FALL_VELOCITY;
			}

			m_moveSpeed.x = m_currentAirMoveSpeed.x;
			m_moveSpeed.y = m_fallVelocity;
			m_moveSpeed.z = m_currentAirMoveSpeed.z;

			m_player->GetCharacterController().Execute(m_moveSpeed, deltaTime);
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());

			if (!m_isHovering)
			{
				m_fallVelocity = m_moveSpeed.y;
			}
		}

		bool PlayerAirAttackState::CheckLanding()
		{
			if (m_attackTimer <= AIR_ATTACK_DURATION || !GET_PLAYER_CHARACON.IsOnGround())
				return false;

			m_isLanding = true;

			if (m_player->GetCurrentWeapon() != WeaponType::Wand && m_player->GetCurrentWeapon() != WeaponType::TwinGun)
				CreateShockWaveEffect();

			return true;
		}

		void PlayerAirAttackState::CreateShockWaveEffect()
		{
			m_landingPosition = m_player->GetWeaponHitDetection().GetPosition();
			m_landingPosition.x += SHOCKWAVE_OFFSET_X;
			m_landingPosition.y = m_player->GetPosition().y;

			m_player->GetEffectList().PlayEffect(nsEffect::ShockWave, m_landingPosition, Quaternion::Identity, Vector3::One * SHOCKWAVE_SCALE);
		}

		bool PlayerAirAttackState::CheckAnimationEndTransition()
		{
			if (m_player->IsPlayAnimation())
			{
				return false;
			}

			m_jumpState = new PlayerJumpState();
			m_jumpState->SetJumpVelocity(m_fallVelocity);
			m_stateMachine->ChangeState(m_jumpState);

			return false;
		}

		Vector3 PlayerAirAttackState::ComputeMissile(float angle)
		{
			m_gunShootDir = m_player->GetForwardVector();
			m_angleY.SetRotationDegY(angle);
			m_angleY.Apply(m_gunShootDir);
			return m_gunShootDir;
		}

		void PlayerAirAttackState::SpawnMissile()
		{
			m_missileShootDir = m_player->GetWeaponHitDetection().GetPosition();
			m_missileShootDir.y += MISSILE_SPAWN_OFFSET_Y;

			for (const float& angle : MISSILE_ANGLE_LIST)
			{
				m_missileShootDir = ComputeMissile(angle);
				m_airMissile = NewGO<nsActor::MagicProjectotile>(0, "AirMissile");
				m_airMissile->Initialize(nsActor::MagicType::enAirMagic, m_missileShootDir, m_missileShootDir);
			}
		}

		void PlayerAirAttackState::FireAirBullet()
		{
			m_spawnPosR = m_player->GetBonePosition(L"mixamorig:RightHand");
			m_spawnPosL = m_player->GetBonePosition(L"mixamorig:LeftHand");

			m_gunShootDir = m_player->GetForwardVector();
			m_gunShootDir.y -= BULLET_SHOOT_DIR_DOWN_OFFSET;
			m_gunShootDir.Normalize();
			
			m_forwardDirection = m_gunShootDir;

			/* 右手から発射。*/
			m_spawnPosition = m_player->GetBonePosition(L"mixamorig:RightHand");
			ConstructAndTransmitBulletRequest(BulletType::enAirial);

			/* 左手から発射。*/
			m_spawnPosition = m_player->GetBonePosition(L"mixamorig:LeftHand");
			ConstructAndTransmitBulletRequest(BulletType::enAirial);

			/* 反動を計算。*/
			m_recoil = m_player->GetForwardVector() * RECOIL_POWER_BACK;
			m_recoil.y += RECOIL_POWER_UP;
			m_recoilVector = m_recoil;
		}
	}
}