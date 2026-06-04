#include "stdafx.h"
#include "PlayerRunState.h"

namespace
{
		const auto MODEL_ANGLE = 90.0f;            /* モデルの回転角度。*/
	const auto MOVE_FRAME_TIME = 1.0f / 60.0f; /* 1フレームあたりの固定時間。*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerRunState::Enter()
		{
			/* プレイヤーのポインタを取得。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Run);
		}


		void PlayerRunState::Update()
		{
			const auto& inputClass = m_player->GetInputClass();

			if (inputClass.IsMove())
			{
				/* 歩行速度より速くする。*/
				SetRunSpeed(m_player->GetRunSpeed());
				SetPosition(m_player->GetPosition());
				SetMoveDirection(inputClass.GetMoveVector());

				m_moveVector = m_moveDirection * m_runSpeed;
				m_player->GetCharacterController().Execute(m_moveVector, MOVE_FRAME_TIME);
				m_player->SetPosition(m_player->GetCharacterController().GetPosition());

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


		bool PlayerRunState::RequestID(uint8_t& id)
		{
			const auto& inputClass = m_player->GetInputClass();

			/* ヘルプ入力があればヘルプ状態に遷移。*/ 
			if (inputClass.IsHelp())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enHelp);
				return true;
			}

			/* 攻撃入力があれば攻撃状態に遷移。*/
			if (inputClass.IsAttack())
			{
				/* 斬り進む状態に。*/
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enPushForward);
				return true;
			}

			/* ジャンプ入力があればジャンプ状態に遷移。*/
			if (inputClass.IsJump())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enJump);
				return true;
			}

			/* 移動入力がなければ待機状態に遷移。*/
			if(inputClass.IsMove() && !inputClass.IsRun())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enWalk);
				return true;
			}

			/* 移動入力がなければ待機状態に遷移。*/
			if (!inputClass.IsMove())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enIdle);
				return true;
			}

			return false;
		}
	}
}