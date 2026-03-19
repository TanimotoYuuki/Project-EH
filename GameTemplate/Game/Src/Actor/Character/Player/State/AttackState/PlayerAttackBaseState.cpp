#include "stdafx.h"
#include "PlayerAttackBaseState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerAttackBaseState::Enter()
		{
			/* 攻撃の種類ごとにキャストを行う。*/
			m_player = static_cast<nsActor::Player*>(m_owner);
		}


		void PlayerAttackBaseState::Update()
		{
			/* タイマーを加算する。*/
			m_attackTimer++;

			/* 入力クラスを取得する。*/
			const auto& inputClass = m_player->GetInputClass();

			/* Bボタンアクション。*/
			if (inputClass.IsAttack())
			{
				/* 
				 *タイマーを加算する。
				 * Bボタンを押すごとにタイマーを加算し、当てはまるなら連続攻撃に繋げる。
				 */
				m_rushCount++;
				
				/* Bボタンが押されていたら予約を入れる。*/
				m_inputRequests[ComboInputType::PressB] = true;
			}

			/* Aボタンアクション。*/
			if (inputClass.IsSlashUp())
				m_inputRequests[ComboInputType::PressLB2] = true;
			else if(inputClass.IsJump())
				m_inputRequests[ComboInputType::PressA] = true;


			if (m_rushCount >= 2)
				m_inputRequests[ComboInputType::RushB] = true;

			if (m_attackTimer > 5 && !m_player->IsPlayAnimation())
			{
				/* Idle状態へ遷移。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
			}
		}


		void PlayerAttackBaseState::Exit()
		{
			/* Stateを抜ける際の処理。*/
			/* 効果音とかエフェクトとかの再生をストップさせる*/
		}


		bool PlayerAttackBaseState::CheckCombo(PLAYER_STATE_ID currentStateID, uint8_t& id)
		{
			/* 地上にいるかどうかを確認。*/ 
			m_isGrounded = m_player->GetCharacterController().IsOnGround();

			/* ステートIDと地上にいるかどうかを検知させる。*/
			const auto& routes = ComboRouteTable::GetRoutes(currentStateID, m_isGrounded);

			for (const auto& route : routes)
			{
				/* テーブルから指定されたものを取り出す。*/
				m_isInputMatch = m_inputRequests[route.inputType];

				/* 時間と入力条件を満たしているか確認。*/
				if (m_attackTimer >= route.cancelTime && m_isInputMatch)
				{
					/* 条件を満たしている場合、次の状態へ遷移する。*/
					id = static_cast<uint8_t>(route.nextStateID);
					return true;
				}
			}

			return false;
		}
	}
}
