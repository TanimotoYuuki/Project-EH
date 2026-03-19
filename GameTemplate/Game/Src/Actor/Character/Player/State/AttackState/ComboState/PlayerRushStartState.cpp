#include "stdafx.h"
#include "PlayerRushStartState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushEndState.h"

namespace
{
	const auto MOVE_FRAME_SPEED = 1.0 / 60.0f; /* 前進するフレーム数。*/
}
	
namespace nsApp
{
	namespace nsState
	{
		void PlayerRushStartState::Enter()
		{
			/* キャラスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);
		}


		void PlayerRushStartState::Update()
		{
			/* タイマーを加算。*/
			m_attackTimer++;

			/* 前進する処理。*/
			MoveForward();

			/* nフレーム待機後、終了判定を取る。*/
			if (m_attackTimer > 5 && !m_player->IsPlayAnimation())
			{
				/* カウンターを加算。*/
				m_loopCount++;

				if (m_loopCount < 3)
				{
					/* アニメーションをループ。*/
					m_player->PlayWeaponAnimation(AttackType::RushAttack_Start);
					m_attackTimer = 0;
				}

				else
					m_stateMachine->ChangeState(new PlayerRushEndState());
			}
		}


		bool PlayerRushStartState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enRushStart, id);
		}


		void PlayerRushStartState::MoveForward()
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
}