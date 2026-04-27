#include "stdafx.h"
#include "PlayerRushStartState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace
{
	const auto MOVE_FRAME_SPEED = 1.0 / 60.0f; /* 前進するフレーム数。*/
}
	
namespace nsApp
{
	namespace nsState
	{

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



		void PlayerRushStartState::Enter()
		{
			/* キャラスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃のタイプを設定する。*/
			m_currentAttackType = AttackType::RushAttack_Start;

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerRushStartState::Update()
		{
			/* タイマーを加算。*/
			m_attackTimer++;

			/* 前進する処理。*/
			MoveForward();

////////////////////////////////////////////////////////////////////////////
			/* @TODO:リファ。*/
			/* ================================================== */
			if (m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				if (m_attackTimer == 10 && !m_isSummoned)
				{
					SummonMissile();
					m_isSummoned = true;
				}

				/* 撃ち終わった後(12F以降)に、アニメが完了したらすぐIdleに戻す */
				if (m_attackTimer > 12 && !m_player->IsPlayAnimation())
				{
					m_stateMachine->ChangeState(new PlayerIdleState());
				}
			}
			else
			{
				if (m_attackTimer == 10 && !m_isSummoned)
				{
					SummonMissile();
					m_isSummoned = true;
				}

				if (m_attackTimer > 10 && !m_player->IsPlayAnimation())
				{
					m_loopCount++;

					if (m_loopCount < 3)
					{
						m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);
						m_attackTimer = 0;
					}
					else
					{
						m_stateMachine->ChangeState(new PlayerRushEndState());
					}
				}
			}
/////////////////////////////////////////////////////////////////////////////////////////
		}

		bool PlayerRushStartState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enRushStart, id);
		}


		void PlayerRushStartState::MoveForward()
		{
			/* 前進を行うのはHammer/Swordのみ。(04/22現在。)*/
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
			if (m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				m_spawnPos = m_player->GetWeaponHitDetection().GetPosition();
				m_spawnPos.y += 10.0f;
				m_spawnPos += m_player->GetForwardVector() * 10.0f;

				auto* rushMagic = NewGO<nsActor::MagicProjectotile>(0, "RushMagic");

				/* 目標を設定。
				 *  ※今はテスト用で他プレイアブルキャラを目標に。
				 * ボスが実装され次第、切り替える。
				 */
				rushMagic->SetTarget(m_player->SearchCharacter());

				/* 魔法の種類を設定する。*/
				rushMagic->Initialize(nsActor::MagicType::enRushMagic, m_spawnPos, m_player->GetForwardVector());
			}
		}
	}
}