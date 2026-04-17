#include "stdafx.h"
#include "PlayerAirAttackState.h"

#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"

#include "Src/Debug/Sandbag.h"
#include "Src/Sound/SoundLister.h"

namespace
{
	const auto MOVE_FRAME_TIME = 1.0f / 60.0f;     //! 1フレームあたりの固定時間。
	const auto AIR_MOVE_SPEED = 120.0f;            //! 空中での前後左右のスピード。
	const auto CHARGE_TIME = 20;				   //! 空中攻撃のチャージ時間。
	const auto AIR_ATTACK_TIMER = 0.0f;			   //! 空中攻撃のタイマーの初期値。
	const auto AIR_ATTACK_DURATION = 25;		   //! 空中攻撃の持続時間。
	const auto FALL_VELOCITY = -15.0f;			   //! 落下速度の初期値。
	const auto GRAVITY = 150.0f;				   //! 重力の値。
	const auto FALL_ACCELERATION = 30.0f;		   //! 落下加速度。
	const auto MAX_FALL_VELOCITY = -1200.0f;	   //! 落下速度の最大値。

	const float ZERO_MOVE_SPEED = 0.0f;            //! 移動速度の初期値。

}	


namespace nsApp
{
	namespace nsState
	{
		void PlayerAirAttackState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃のタイプを設定する。*/
			m_currentAttackType = AttackType::AirAttack;

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::AirAttack);

			/* 攻撃タイマーを初期化。*/
			SetAttackTimer(AIR_ATTACK_TIMER); 

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerAirAttackState::Update()
		{
			/* タイマーを加算する。*/
			m_attackTimer++;

			/* 未着地の場合。*/
			if (!m_isLanding)
			{
				/* 空中での移動処理。*/
				UpdateAirMovement();

				/* 着地判定。*/
				CheckLanding();
			}

			/* 着地しているなら*/
			else
			{
				/* アニメーションが再生し終わるまで待つ。*/
				if (!m_player->IsPlayAnimation())
					m_stateMachine->ChangeState(new PlayerIdleState());
			}
		}


		void PlayerAirAttackState::UpdateAirMovement()
		{
			/* 毎フレーム移動速度をリセットする。*/
			m_currentAirMoveSpeed = Vector3::Zero;

			/* 空中でも左右に移動出来るようにする。*/
			const auto& inputClass = m_player->GetInputClass();
			if (inputClass.IsMove())
			{
				m_currentAirMoveSpeed.x = inputClass.GetMoveVector().x * AIR_MOVE_SPEED;
				m_currentAirMoveSpeed.z = inputClass.GetMoveVector().z * AIR_MOVE_SPEED;
			}

			if (m_attackTimer >= AIR_ATTACK_DURATION)
				m_fallVelocity -= FALL_ACCELERATION;
			else
				m_fallVelocity = 0.0f;

			/* ステージにめり込まないように制限。*/
			if (m_fallVelocity < MAX_FALL_VELOCITY)
				m_fallVelocity = MAX_FALL_VELOCITY;


			/* 移動速度を設定。*/
			SetMoveSpeed(Vector3(m_currentAirMoveSpeed.x, m_fallVelocity, m_currentAirMoveSpeed.z));

			/* 移動速度をキャラコンに反映。*/
			m_player->GetCharacterController().Execute(m_moveSpeed, MOVE_FRAME_TIME);

			/* 座標にも反映。*/
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());

			/* Y軸の速度を変数に代入。*/
			m_fallVelocity = m_moveSpeed.y;
		}


		bool PlayerAirAttackState::CheckLanding()
		{
			/* 未着地の場合はfalseを返す。*/
			if (m_attackTimer <= AIR_ATTACK_DURATION || !m_player->GetCharacterController().IsOnGround())
				return false;

			/* */
			m_isLanding = true;

			/* 衝撃波エフェクトを生成する。*/
			CreateShockWaveEffect();

			return true;
		}


		void PlayerAirAttackState::CreateShockWaveEffect()
		{
			/* 武器の当たり判定クラスの座標を取得する。*/
			m_landingPosition = m_player->GetWeaponHitDetection().GetPosition();

			/* それぞれの軸で値を代入する。*/
			m_landingPosition.x = m_landingPosition.x + 50.0f;
			m_landingPosition.y = m_player->GetPosition().y;
			m_landingPosition.z = m_landingPosition.z;

			/* エフェクトを生成する。*/
			m_player->GetEffectList().PlayEffect(nsEffect::ShockWave, m_landingPosition, Quaternion::Identity, Vector3::One * 2.0f);
		}


		bool PlayerAirAttackState::CheckAnimationEndTransition()
		{
			/* アニメーションの終了判定。*/
			if (m_player->IsPlayAnimation())
				return false;

			/* ジャンプが終わっている場合、待機状態に戻す。*/
			auto jumpState = new PlayerJumpState();
			jumpState->SetJumpVelocity(m_fallVelocity);
			m_stateMachine->ChangeState(jumpState);

			return false;
		}



	}
}