#include "stdafx.h"
#include "PlayerWalkState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/PlayerInput.h"

namespace
{
	const auto WALK_SPEED = 1.0;    /* 歩行速度。*/
	const auto MODEL_ANGLE = 90.0f; /* モデルの回転角度。*/
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

			/* */
			if (inputClass.IsMove())
			{
				/* 歩行速度をセット。*/
				SetWalkSpeed(WALK_SPEED);

				m_currentPosition = m_player->GetPosition();
				m_moveDirection = inputClass.GetMoveVector();

				/* 現在の座標にスティックの入力量と速度を加算。*/
				m_player->SetPosition(m_currentPosition + (m_moveDirection * m_walkSpeed));


                /* 回転軸を制御。*/
				if (m_moveDirection.x > 0.01f)
					m_player->SetAngle(MODEL_ANGLE);

				else if(m_moveDirection.x < -0.01f)
					m_player->SetAngle(-MODEL_ANGLE);
			}
		}


		bool PlayerWalkState::RequestID(uint8_t& id)
		{
			const auto& inputClass = m_player->GetInputClass();

			/* 攻撃ボタンが押されたら攻撃ステートへ。*/
			if (inputClass.IsAttack())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enAttack);
				return true;
			}

			if (inputClass.IsJump())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enJump);
				return true;
			}

			/* スティックの入力量が無くなったら待機ステートに戻る。*/
			if (!inputClass.IsMove())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enIdle);
				return true;
			}

			if (inputClass.IsRun())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enRun);
				return true;
			}

			return false;
		}
	}
}