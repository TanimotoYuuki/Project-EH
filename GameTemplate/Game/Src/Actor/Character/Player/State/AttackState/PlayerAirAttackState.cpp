#include "stdafx.h"
#include "PlayerAirAttackState.h"

#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"

namespace
{
	const auto MOVE_FRAME_TIME = 1.0f / 60.0f;     //! 1フレームあたりの固定時間。
	const auto AIR_MOVE_SPEED = 120.0f;            //! 空中での前後左右のスピード。
	const float ZERO_MOVE_SPEED = 0.0f;            //! 移動速度の初期値。
	const auto CHARGE_TIME = 20;				   //! 空中攻撃のチャージ時間。
}	

namespace nsApp
{
	namespace nsState
	{
		void PlayerAirAttackState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::AirAttack);

			/* 攻撃タイマーを初期化。*/
			SetAttackTimer(0.0f); 

			/* 多段ジャンプを防止するためジャンプ力を引き継がない。*/
			if (m_fallVelocity == 0.0f)
				SetFallVelocity(150.0f);
		}


		void PlayerAirAttackState::Update()
		{
			/* タイマーを加算する。*/
			m_attackTimer++;

			if (m_attackTimer < CHARGE_TIME)
			{
				m_fallVelocity -= 5.0f;
				if(m_fallVelocity < ZERO_MOVE_SPEED)
					m_fallVelocity = ZERO_MOVE_SPEED;
			}

			else
				m_fallVelocity -= 200.0f;


			/* */
			if (m_attackTimer < 60)
				m_fallVelocity -= 10.0f;

			else
				m_fallVelocity -= 150.0f;

			/* ステージにめり込まないように制限。*/
			if(m_fallVelocity < -1200.0)
				m_fallVelocity = -1200.0f;

			/* 空中でもスティックで左右に動けるようにする。*/
			const auto& inputClass = m_player->GetInputClass();
			if (inputClass.IsMove())
			{
				m_currentAirMoveSpeed.x = inputClass.GetMoveVector().x * AIR_MOVE_SPEED;
				m_currentAirMoveSpeed.z = inputClass.GetMoveVector().z * AIR_MOVE_SPEED;
			}

			/* 移動速度を設定。*/
			SetMoveSpeed(Vector3(m_currentAirMoveSpeed.x, m_fallVelocity, m_currentAirMoveSpeed.z));

			/* 移動速度をキャラコンに反映。*/
			m_player->GetCharacterController().Execute(m_moveSpeed, MOVE_FRAME_TIME);

			/* 座標にも反映。*/
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());

			/* Y軸の速度を変数に代入。*/
			m_fallVelocity = m_moveSpeed.y;

			/* 攻撃の途中で着地したら待機状態に戻す。*/
			if (m_player->GetCharacterController().IsOnGround())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return;
			}

			/* 空中でアニメーションが終わったら落下状態にジャンプ状態に戻す。*/
			if (m_attackTimer > 60 && !m_player->IsPlayAnimation())
			{
				auto jumpState = new PlayerJumpState();
				jumpState->SetJumpVelocity(m_fallVelocity);
				m_stateMachine->ChangeState(jumpState);
			}
		}
	}
}