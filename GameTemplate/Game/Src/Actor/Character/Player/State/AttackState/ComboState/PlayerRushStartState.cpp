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
	const auto ATTACK_TIMER_10 = 10;           //! 攻撃タイマーの10フレーム目。
	const auto ATTACK_TIMER_12 = 12;           //! 攻撃タイマーの12フレーム目。

	const auto LOOP_COUNT_4 = 4;               //! ループ回数。
	const auto LOOP_COUNT_0 = 0;               //! ループ回数。

	const auto WEAPON_ANGLE = 90.0f;           //! 武器の角度。
	const auto MOVE_SPEED = 50.0f;             //! 移動速度。
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
		void PlayerRushStartState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定する。*/
			SetCurrentAttackType(AttackType::RushAttack_Start);
			m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);

			/* 武器が銃の場合。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				/* サブウェポンの描画を設定。*/
				m_player->LoadSubWeapon(CharacterModelType::Weapon_TwinGun);
				/* 武器の角度を設定。*/ 
				m_player->SetWeaponRotationAngle(Vector3::Front, WEAPON_ANGLE);
			}
		}


		void PlayerRushStartState::OnEnterAttack()
		{
			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();

			/* 攻撃タイマーをリセット。*/
			m_loopCount = LOOP_COUNT_0;
		}


		bool PlayerRushStartState::OnUpdateAttack()
		{
			/* 移動処理。*/
			MoveForward();

			/* 銃の場合。*/
			if(m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				/* アニメーションの終了判定。*/
				if (!m_player->IsPlayAnimation())
				{
					/* count。*/
					m_loopCount++;

					/* Bボタンの判定を取得。*/
					m_isReleased = !m_player->GetInputClass().CheckButtonPress(enButtonB);

					/* countが4回以上なら。*/
					if (m_loopCount >= LOOP_COUNT_4 || m_isReleased)
					{
						/* 連続攻撃終了ステートへ遷移。*/
						m_stateMachine->ChangeState(new PlayerRushEndState());
						return true;
					}

					else
					{
						/* アニメーションを再生。*/
						m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);
						/* 攻撃タイマーをリセット。*/
						SetAttackTimer(0);
					}
				}
			}

			else if (m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				if (m_attackTimer > ATTACK_TIMER_12 && !m_player->IsPlayAnimation())
				{
					m_stateMachine->ChangeState(new PlayerIdleState());
					return true;
				}
			}

			return false;
		}


		void PlayerRushStartState::OnExitAttack()
		{
			/* 銃の場合。*/
			if (m_player && m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				/* サブウェポンの描画をリセット。*/
				m_player->ResetSubWeapon();
				/* 武器の角度をリセット。*/
				m_player->SetWeaponRotationAngle(Vector3::Front, -WEAPON_ANGLE);
			}
		}


		void PlayerRushStartState::OnAttackTick()
		{
			/* 内部的にAttackTimerをチェック。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				AdjustBoneNameByKeyFrameNumber(); 

			/* 武器種が杖の場合。*/
			else if (m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				/* 攻撃タイマーの10フレーム目で、乱射弾を生成する。*/
				if (m_attackTimer == ATTACK_TIMER_10 && !m_isSummoned)
				{
					/* 魔法を生成する。*/
					ConstructAndTransmitMagicRequest(nsActor::MagicType::enRushMagic, m_player->SearchCharacter());
					m_isSummoned = true;
				}
			}
		}


		bool PlayerRushStartState::OnRequestAttackID(uint8_t& id)
		{
			/* 乱射の開始攻撃のIDを返す。*/
			return CheckCombo(nsActor::PlayerStateID::enRushStart, id);
		}


		void PlayerRushStartState::MoveForward()
		{
			/* 前進を行うのはHammer/Swordのみ。*/
			if (m_player->GetCurrentWeapon() == WeaponType::GreatSword or m_player->GetCurrentWeapon() == WeaponType::Hammer)
			{
				/* 前進。*/
				m_forwardSpeed = MOVE_SPEED;

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
			/* 攻撃タイマーの5フレーム目と12フレーム目で、乱射弾を生成する。*/
			if (m_attackTimer == ATTACK_TIMER_5)
				FireRushBullet(L"mixamorig:RightHand");

			/* 12フレーム目の攻撃タイマーのとき、左手から乱射弾を生成する。*/
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