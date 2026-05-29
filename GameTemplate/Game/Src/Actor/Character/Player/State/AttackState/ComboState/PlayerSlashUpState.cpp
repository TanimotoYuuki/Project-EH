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
		void PlayerSlashUpState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定。*/
			m_currentAttackType = AttackType::SlashUp;

			/* 攻撃アニメーションを再生する。*/
			m_player->PlayWeaponAnimation(AttackType::SlashUp);
		}


		void PlayerSlashUpState::OnEnterAttack()
		{
			/* 武器の角度を設定。*/
			m_player->SetWeaponRotationAngle(Vector3::Right, 0.0f);
			/* 初速を設定。*/
			SetJumpVelocity(200.0f);
			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		bool PlayerSlashUpState::OnUpdateAttack()
		{
			/* 杖のエフェクト。*/
			if (m_player->GetCurrentWeapon() == WeaponType::Wand && m_attackTimer == 10 && !m_isSummoned)
			{
				/* Playerクラスの座標を取得。*/
				m_effectPosition = m_player->GetPosition();
				m_effectPosition.y += 10.0f;
				/* 再生するエフェクトを設定。*/
				m_player->GetEffectList().PlayEffect(nsEffect::ShockWave, m_effectPosition, Quaternion::Identity, Vector3::One * 2.0f);
				m_isSummoned = true;
			}

			/* 上昇/重力処理。*/
			m_jumpVelocity -= 30.0f;
			if (m_jumpVelocity < -1200.0f)
				m_jumpVelocity = -1200.0f;

			/* 移動速度を設定。*/
			m_moveSpeed = Vector3(0.0f, m_jumpVelocity, 0.0f);
			m_player->GetCharacterController().Execute(m_moveSpeed, MOVE_FRAME_TIME);
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());
			m_jumpVelocity = m_moveSpeed.y;

			/* アニメーション終了後にジャンプステートへ遷移。*/
			if (m_attackTimer > 5 && !m_player->IsPlayAnimation())
			{
				TransitionJumpState();
				return true;
			}

			return false;
		}


		void PlayerSlashUpState::OnExitAttack()
		{
			m_player->SetWeaponRotationAngle(Vector3::Right, 0.0f);
		}


		bool PlayerSlashUpState::OnRequestAttackID(uint8_t& id)
		{
			/* Bボタン入力で空中攻撃に遷移。*/
			if (m_inputRequests[ComboInputType::PressB])
				TransitionAirAttack();

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