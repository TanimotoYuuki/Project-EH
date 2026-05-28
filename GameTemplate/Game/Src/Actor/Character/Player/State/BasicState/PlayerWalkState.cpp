#include "stdafx.h"
#include "PlayerWalkState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Player/Component/StateTransitionDiagram.h"

namespace
{
	const auto WALK_SPEED = 50;                /* 歩行速度。*/
	const auto MODEL_ANGLE = 90.0f;            /* モデルの回転角度。*/
	const auto MOVE_FRAME_TIME = 1.0f / 60.0f; /* 1フレームあたりの固定時間。*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerWalkState::Enter()
		{
			/* キャスト処理。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 歩行アニメーションを再生する。*/ 
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Walk);
		}


		void PlayerWalkState::Update()
		{
			/* 入力判定クラスを取得。*/
			const auto& inputClass = m_player->GetInputClass();

			/* 動作判定を確認。*/
			if (inputClass.IsMove())
			{
				/* 歩行速度をセット。*/
				SetWalkSpeed(WALK_SPEED);

				/* 現在の座標を取得する。*/
				m_currentPosition = m_player->GetPosition();
				m_moveDirection = inputClass.GetMoveVector();

				/* 現在の座標にスティックの入力量と速度を加算。*/
				m_moveVector = m_moveDirection * m_walkSpeed;
				m_player->MoveWithBattleClamp(m_moveVector, MOVE_FRAME_TIME);

				/* 回転軸を制御。*/
				if (m_moveDirection.x > 0.01f)
				{
					m_player->SetAngle(MODEL_ANGLE);
					m_player->SetForwardVector(Vector3::Right);
				}

				else if (m_moveDirection.x < -0.01f)
				{
					m_player->SetAngle(-MODEL_ANGLE);
					m_player->SetForwardVector(Vector3::Left);
				}
			}
		}


		bool PlayerWalkState::RequestID(uint8_t& id)
		{
			/* 状態遷移全体を確認。*/
			if (StateTransitionDiagram::CheckCommonTransition(m_player->GetInputClass(), id))
				return true;

			if (!m_player->GetInputClass().IsMove())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enIdle);
				return true;
			}

			return false;
		}
	}
}