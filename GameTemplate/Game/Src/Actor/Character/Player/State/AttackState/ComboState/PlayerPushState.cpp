#include "stdafx.h"
#include "PlayerPushState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace
{
	const auto MOVE_SPEED_FRAME = 1.0f /40.0f; //! 突き進む攻撃の移動速度。
	const auto MOVE_SPEED = 40.0f;             //! 突き進む攻撃の移動速度。
	const auto START_WEAPON_ANGLE = -90.0f;    //! ステート開始時の武器の角度。
	const auto END_WEAPON_ANGLE = 0.0f;        //! ステート終了時の武器の角度。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerPushState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定。*/
			m_currentAttackType = AttackType::PushForward;
			if (m_player->GetCurrentWeapon() != WeaponType::TwinGun)
				m_player->PlayWeaponAnimation(AttackType::PushForward);
		}


		void PlayerPushState::OnEnterAttack()
		{
			/* 武器の角度を設定。*/
			if (m_player->GetCurrentWeapon() != WeaponType::TwinGun)
				m_player->SetWeaponRotationAngle(Vector3::Front, -90.0f);

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		bool PlayerPushState::OnUpdateAttack()
		{
			/* 前進処理。*/
			MoveForward();

			/* 銃の発射タイミング。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun && m_attackTimer == 10)
			{
				m_spawnPosition = m_player->GetBonePosition(L"mixamorig:RightHand");
				m_forwardDirection = m_player->GetForwardVector();
				ConstructAndTransmitBulletRequest(BulletType::enDash);
			}

			return false;
		}


		void PlayerPushState::OnExitAttack()
		{
			if (m_player->GetCurrentWeapon() != WeaponType::TwinGun)
				m_player->SetWeaponRotationAngle(Vector3::Front, -90.0f);
		}


		bool PlayerPushState::OnRequestAttackID(uint8_t& id)
		{
			/* コンボテーブル判定。*/
			if (CheckCombo(nsActor::PlayerStateID::enPushForward, id))
				return true;

			/* 銃の場合。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun && m_attackTimer > 20)
			{
				/* 入力システムクラスを取得。*/
				auto& inputClass = m_player->GetInputClass();

				/* 走り判定。*/
				if (inputClass.IsRun())
					m_stateMachine->ChangeState(new PlayerRunState());

				/* 移動判定。*/
				else if (inputClass.IsMove())
					m_stateMachine->ChangeState(new PlayerWalkState());

				else
					m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			return false;
		}


		void PlayerPushState::MoveForward()
		{
			if (m_attackTimer <= 12)
				/* 前進する速度を設定。*/
				SetForwardSpeed(MOVE_SPEED);

			else
			{
				/* 減衰処理。*/
				m_forwardSpeed *= 0.8f;

				if (m_forwardSpeed < 0.1f)
					m_forwardSpeed = 0.0f;
			}

			/* 前方向のベクトルを取得。*/
			m_moveVector = m_player->GetForwardVector() * m_forwardSpeed;
			/* 当たり判定の移動。*/
			/* キャラコンの移動。*/
			m_player->GetCharacterController().Execute(m_moveVector, MOVE_SPEED_FRAME);
			/* 座標の移動。*/
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());
		}


		void PlayerPushState::FireDashBullet()
		{
			m_spawnPosition = m_player->GetBonePosition(L"mixamorig:RightHand");
			BulletFactory::CreateBullet(BulletType::enDash, m_spawnPosition, m_player->GetForwardVector());
		}


		bool PlayerPushState::TransitionMultiState()
		{
			if (m_attackTimer > 20)
			{
				auto& inputClass = m_player->GetInputClass();
				if (inputClass.IsRun())
					m_stateMachine->ChangeState(new nsState::PlayerRunState());

				else if (inputClass.IsMove())
					m_stateMachine->ChangeState(new nsState::PlayerWalkState());

				else
					m_stateMachine->ChangeState(new nsState::PlayerIdleState());

				return true;
			}

			return false;
		}
	}
}