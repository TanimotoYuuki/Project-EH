#include "stdafx.h"
#include "PlayerSlashUpState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"

namespace
{
	const auto MOVE_FRAME_TIME = 1.0f / 60.0f; /* 1フレームあたりの固定時間。*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerSlashUpState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::SlashUp);

			/**/
			m_player->SetWeaponRotationAngle(Vector3::Right, 0.0f);

			/* 初速を設定。*/
			SetJumpVelocity(200.0f);
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerSlashUpState::Update()
		{
			m_attackTimer++;
			const auto& inputClass = m_player->GetInputClass();

			/* Bボタンが押されたことを知らせる。*/
			if(inputClass.IsAirAttack() || inputClass.IsAttack())
				m_inputRequests[ComboInputType::PressB] = true;


			/* 斬り上げ時の上昇と落下処理。*/
			m_jumpVelocity -= 30.0f; /* 重力を設定。*/
			/* ステージにめり込まないように制限。*/
			if (m_jumpVelocity < -1200.0f)
				m_jumpVelocity = -1200.0f;

			m_slashUpSpeed = Vector3(0.0f, m_jumpVelocity, 0.0f);
			m_player->GetCharacterController().Execute(m_slashUpSpeed, MOVE_FRAME_TIME);
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());
			m_jumpVelocity = m_slashUpSpeed.y;


			/* 空中攻撃状態に遷移。*/
			if (m_inputRequests[ComboInputType::PressB])
			{
				TransitionAirAttack();
				return;
			}

			/* ジャンプ状態に遷移。*/
			if (m_attackTimer > 5 && !m_player->IsPlayAnimation())
			{
				TransitionJumpState();
				return;
			}
		}


		void PlayerSlashUpState::Exit()
		{
			m_player->SetWeaponRotationAngle(Vector3::Right, 0.0f);
		}

		bool PlayerSlashUpState::RequestID(uint8_t& id)
		{
			return false;
		}


		void PlayerSlashUpState::TransitionAirAttack()
		{
			/* 空中攻撃ステートを生成。*/
			auto airAttack = new PlayerAirAttackState();
			/* 初速を設定。*/
			airAttack->SetFallVelocity(m_jumpVelocity);
			/* ステートを設定。*/
			m_stateMachine->ChangeState(airAttack);
		}


		void PlayerSlashUpState::TransitionJumpState()
		{
			/* ジャンプ状態に遷移。*/
			auto jumpState = new PlayerJumpState();
			/* 初速を設定。*/
			jumpState->SetJumpVelocity(m_jumpVelocity);
			/* ステートを設定。*/
			m_stateMachine->ChangeState(jumpState);
		}
	}
}