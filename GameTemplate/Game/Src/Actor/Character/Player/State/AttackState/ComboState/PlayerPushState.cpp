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

	const auto ATTACK_TIMER_10 = 10;           //! 攻撃タイマーの10フレーム目。
	const auto ATTACK_TIMER_12 = 12;           //! 攻撃タイマーの12フレーム目。
	const auto ATTACK_TIMER_20 = 20;           //! 攻撃タイマーの20フレーム目。
	const auto DAMQING_RATIO = 0.8f;           //! 移動速度の減衰倍率。
	const auto FORWARD_SPEED_THRESHOLD = 0.1f; //! 前進速度の閾値。
	const auto FORWARD_SPEED_CLIPPING = 0.0f; //! 前進速度の閾値。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerPushState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定する。*/
			SetCurrentAttackType(AttackType::PushForward);

			/* アニメーション再生。*/
			if (m_player->GetCurrentWeapon() != WeaponType::TwinGun)
				m_player->PlayWeaponAnimation(AttackType::PushForward);
		}


		void PlayerPushState::OnEnterAttack()
		{
			/* 武器の角度を設定。*/
			if (m_player->GetCurrentWeapon() != WeaponType::TwinGun)
				m_player->SetWeaponRotationAngle(Vector3::Front, START_WEAPON_ANGLE);

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		bool PlayerPushState::OnUpdateAttack()
		{
			/* 前進処理。*/
			MoveForward();

			/* 銃の発射タイミング。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun && m_attackTimer == ATTACK_TIMER_10)
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
				m_player->SetWeaponRotationAngle(Vector3::Front, START_WEAPON_ANGLE);
		}


		bool PlayerPushState::OnRequestAttackID(uint8_t& id)
		{
			/* コンボテーブル判定。*/
			if (CheckCombo(nsActor::PlayerStateID::enPushForward, id))
				return true;

			/* 銃の場合。*/
			if (m_player->GetCurrentWeapon() == WeaponType::TwinGun && m_attackTimer > ATTACK_TIMER_20)
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
			if (m_attackTimer <= ATTACK_TIMER_12)
				/* 前進する速度を設定。*/
				SetForwardSpeed(MOVE_SPEED);

			else
			{
				/* 減衰処理。*/
				m_forwardSpeed *= DAMQING_RATIO;

				/* 前進速度の閾値処理。*/
				if (m_forwardSpeed < FORWARD_SPEED_THRESHOLD)
					m_forwardSpeed = FORWARD_SPEED_CLIPPING;
			}

			/* 前方向のベクトルを取得。*/
			m_moveVector = m_player->GetForwardVector() * m_forwardSpeed;
			/* キャラコンの移動。*/
			m_player->GetCharacterController().Execute(m_moveVector, MOVE_SPEED_FRAME);
			/* 座標の移動。*/
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());
		}


		void PlayerPushState::FireDashBullet()
		{
			/* 銃の発射タイミング。*/
			m_spawnPosition = m_player->GetBonePosition(L"mixamorig:RightHand");
			BulletFactory::CreateBullet(BulletType::enDash, m_spawnPosition, m_player->GetForwardVector());
		}


		bool PlayerPushState::TransitionMultiState()
		{
			if (m_attackTimer > ATTACK_TIMER_20)
			{
				/* 入力システムクラスを取得。*/
				auto& inputClass = m_player->GetInputClass();

				/* 走り判定。*/
				if (inputClass.IsRun())
					m_stateMachine->ChangeState(new nsState::PlayerRunState());

				/* 移動判定。*/
				else if (inputClass.IsMove())
					m_stateMachine->ChangeState(new nsState::PlayerWalkState());

				/* 何も入力されていない場合はIdleに遷移。*/
				else
					m_stateMachine->ChangeState(new nsState::PlayerIdleState());

				return true;
			}

			return false;
		}
	}
}