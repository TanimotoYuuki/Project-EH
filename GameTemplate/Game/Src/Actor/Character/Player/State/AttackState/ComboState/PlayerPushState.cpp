#include "stdafx.h"
#include "PlayerPushState.h"

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

			/* キャスト。*/
			m_player->PlayWeaponAnimation(AttackType::PushForward);

			/* 武器の角度を調整。*/
			m_player->SetWeaponRotationAngle(Vector3::Front, START_WEAPON_ANGLE);
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerPushState::Update()
		{
			/* 前進する。*/
			MoveForward();

			/* 親クラスの更新。*/
			PlayerAttackBaseState::Update();
		}


		void PlayerPushState::Exit()
		{
			/* ステート終了時の武器の角度を設定する。*/
			m_player->SetWeaponRotationAngle(Vector3::Right, END_WEAPON_ANGLE);
			/* ステートを終了する。*/
			PlayerAttackBaseState::Exit();
		}


		void PlayerPushState::MoveForward()
		{
			/* 前進する速度を設定。*/
			SetForwardSpeed(MOVE_SPEED);

			/* 前方向のベクトルを取得。*/
			m_moveVector = m_player->GetForwardVector() * m_forwardSpeed;

			/* 当たり判定の移動。*/
			/* キャラコンの移動。*/
			m_player->GetCharacterController().Execute(m_moveVector, MOVE_SPEED_FRAME);
			/* 座標の移動。*/
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());
		}


		bool PlayerPushState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enPushForward, id);
		}
	}
}