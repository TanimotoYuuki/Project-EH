#include "stdafx.h"
#include "PlayerRushStartState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace
{
	const auto MOVE_FRAME_SPEED = 1.0 / 60.0f; //! 前進するフレーム数。
	const auto INCREASE_VALUE_Y = 10.0f;       //! Y軸の上昇値。
	const auto ATTACK_TIMER_5 = 5;             //! 攻撃タイマーの5フレーム目。
	const auto ATTACK_TIMER_12 = 5;            //! 攻撃タイマーの12フレーム目。
	const auto WEAPON_ANGLE = -90.0f;          //! 武器の角度。
}
	
/** @def
 * @brief Playerクラスの現在の武器を取得するマクロ。
 */
#define GET_PLAYER_WEAPON m_player->GetCurrentWeapon()

/** @def
 * @brief 再生するアニメーションを設定するマクロ。
 */
#define PLAYER_PLAY_ANIMATION m_player->PlayWeaponAnimation

/** @def
 * @brief Playerの前方向のベクトルを取得するマクロ。
 */
#define GET_PLAYER_FORWARD_VECTOR m_player->GetForwardVector()


namespace nsApp
{
	namespace nsState
	{
		void PlayerRushStartState::Enter()
		{
			/* キャラスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃のタイプを設定する。*/
			m_currentAttackType = AttackType::RushAttack_Start;

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);

			/* GunCharacterの場合、もう一つ銃モデルをロードする。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				m_player->LoadSubWeapon(CharacterModelType::Weapon_TwinGun);
				m_player->SetWeaponRotationAngle(Vector3::Front, -WEAPON_ANGLE);
			}

			/* 当たり判定を有効にする。*/ 
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerRushStartState::Update()
		{
			/* 前進する処理。*/
			MoveForward();

			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				/* ボーンの切り替え。*/
				AdjustBoneNameByKeyFrameNumber();

				/* ループと終了の判定 */
				if (!m_player->IsPlayAnimation())
				{
					m_loopCount++;
					m_isButtonReleased = !m_player->GetInputClass().CheckButtonPress(enButtonB);

					if (m_loopCount >= 4 || m_isButtonReleased)
					{
						m_stateMachine->ChangeState(new PlayerRushEndState());
						return; // ★追加：自分が消滅するので即座にリターン！
					}
					else
					{
						m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);
						m_attackTimer = 0;
					}
				}
			}
			else if (m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				if (m_attackTimer == 10 && !m_isSummoned) {
					SummonMissile();
					m_isSummoned = true;
				}

				if (m_attackTimer > ATTACK_TIMER_12 && !m_player->IsPlayAnimation())
				{
					m_stateMachine->ChangeState(new PlayerIdleState());
					return; 
				}
			}
			else
			{
				if (m_attackTimer == 10 && !m_isSummoned) {
					SummonMissile();
					m_isSummoned = true;
				}

				if (m_attackTimer > 10 && !m_player->IsPlayAnimation())
				{
					m_loopCount++;
					if (m_loopCount < 3) {
						m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);
						m_attackTimer = 0;
					}
					else
					{
						m_stateMachine->ChangeState(new PlayerRushEndState());
						return; 
					}
				}
			}

			PlayerAttackBaseState::Update();
		}


		void PlayerRushStartState::Exit()
		{
			if (m_player && m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				m_player->ResetSubWeapon();
				m_player->SetWeaponRotationAngle(Vector3::Front ,WEAPON_ANGLE);
			}

			PlayerAttackBaseState::Exit();
		}


		bool PlayerRushStartState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enRushStart, id);
		}


		void PlayerRushStartState::MoveForward()
		{
			/* 前進を行うのはHammer/Swordのみ。*/
			if (m_player->GetCurrentWeapon() == WeaponType::GreatSword or m_player->GetCurrentWeapon() == WeaponType::Hammer)
			{
				/* 前進。*/
				m_forwardSpeed = 50.0f;

				m_moveVector = m_player->GetForwardVector() * m_forwardSpeed;

				/* 当たり判定の移動。*/
				m_player->GetCharacterController().Execute(m_moveVector, MOVE_FRAME_SPEED);
				/* プレイヤーの座標をキャラコンの座標に合わせる。*/
				m_player->SetPosition(m_player->GetCharacterController().GetPosition());
			}
		}


		void PlayerRushStartState::SummonMissile()
		{
			/* 魔法を生成する。*/
			if (m_player->GetCurrentWeapon() == WeaponType::Wand)
				ConstructAndTransmitMagicRequest(nsActor::MagicType::enRushMagic, m_player->SearchCharacter());
		}


		void PlayerRushStartState::AdjustBoneNameByKeyFrameNumber()
		{
			if (m_attackTimer == ATTACK_TIMER_5)
				FireRushBullet(L"mixamorig:RightHand");

			else if(m_attackTimer == ATTACK_TIMER_12)
				FireRushBullet(L"mixamorig:LeftHand");
		}


		void PlayerRushStartState::FireRushBullet(const wchar_t* boneName)
		{
			/* ボーンの位置を取得する。*/
			m_spawnPosition = m_player->GetBonePosition(boneName);
			m_forwardDirection = m_player->GetForwardVector();

			/* 乱射弾（enRush）を指定。*/
			ConstructAndTransmitBulletRequest(BulletType::enRush);
		}
	}
}