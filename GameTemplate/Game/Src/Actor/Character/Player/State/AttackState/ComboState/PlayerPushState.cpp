#include "stdafx.h"
#include "PlayerPushState.h"
#include "Src/Actor/Gun/Bullet/DashBullet.h"
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
		void PlayerPushState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃のタイプを設定する。*/
			m_currentAttackType = AttackType::PushForward;

			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
			}
			else
			{
				/* 他の武器（剣やハンマー）の時は既存の処理を行う */
				m_player->PlayWeaponAnimation(AttackType::PushForward);
				m_player->SetWeaponRotationAngle(Vector3::Front, START_WEAPON_ANGLE);
			}
			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerPushState::Update()
		{
			if (!m_player)
				return;

			/* 前進する。*/
			MoveForward();

			/* 武器が銃の場合。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
			{
				/* 弾を発射。*/
				if (m_attackTimer == 10)
					FireDashBullet();

				/* 減衰率に応じてステートを遷移し分ける。*/
				if (TransitionMultiState())
					return;
			}

			/* 親クラスの更新。*/
			PlayerAttackBaseState::Update();
		}


		void PlayerPushState::Exit()
		{
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun) {}

			else
			{
				/* 他の武器（剣やハンマー）の時は既存の処理を行う */
				m_player->PlayWeaponAnimation(AttackType::PushForward);
				m_player->SetWeaponRotationAngle(Vector3::Front, START_WEAPON_ANGLE);
			}
			/* ステートを終了する。*/
			PlayerAttackBaseState::Exit();
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


		bool PlayerPushState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enPushForward, id);
		}
	}
}