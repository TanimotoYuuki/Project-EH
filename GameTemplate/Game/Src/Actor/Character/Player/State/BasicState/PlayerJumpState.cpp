#include "stdafx.h"
#include "PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"
#include "Src/Actor/Character/Player/Component/StateTransitionDiagram.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerSlashUpState.h"

namespace
{
	const auto JUMP_POWER = 500.0f;            /* ジャンプの初速（高さ）。*/
	const auto MOVE_FRAME_SPEED = 1.0 / 60.0f; /* 前進するフレーム数。*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerJumpState::Enter()
		{
			/* プレイヤーのポインタを取得する。 */
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* ジャンプアニメーションを再生する。 */
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Jump);

			if (m_player->GetCharacterController().IsOnGround())
			{
				/* 初速を設定。 */
				m_jumpVelocity = JUMP_POWER;
			}
		}


		void PlayerJumpState::Update()
		{
			/* 毎フレーム、移動速度をゼロにリセットする。 */
			m_moveSpeed = Vector3::Zero;
			/* 入力情報を取得する。 */
			const auto& inputClass = m_player->GetInputClass();

			/* 空中でのボタンアクションで攻撃状態に遷移。*/
			if (inputClass.IsAirAttack())
			{
				auto airAttack = new PlayerAirAttackState();
				airAttack->SetFallVelocity(m_jumpVelocity);
				m_stateMachine->ChangeState(airAttack);
				return;
			}


			/* スティック入力があれば、空中のX・Z軸の移動量を計算する。 */
			if (inputClass.IsMove())
			{
				float airMoveSpeed = 120.0f; /* 空中での前後左右のスピード。 */
				m_moveSpeed.x = inputClass.GetMoveVector().x * airMoveSpeed;
				m_moveSpeed.z = inputClass.GetMoveVector().y * airMoveSpeed;
			}

			/* 空中でスティック上 + Aを押した斬り上げ状態に*/
			if (inputClass.IsSlashUp())
			{
				m_stateMachine->ChangeState(new PlayerSlashUpState());
				return;
			}

			/* 重力計算（毎フレーム固定の力で下へ引っ張る）。 */
			float gravity = 30.0f;
			m_jumpVelocity -= gravity;

			/* 落下スピードの限界を設定（すり抜けなどのバグ防止）。 */
			if (m_jumpVelocity < -1200.0f)
				m_jumpVelocity = -1200.0f;

			/* 計算したY軸の速度を移動速度ベクトルに設定する。 */
			m_moveSpeed.y = m_jumpVelocity;

			/* キャラクターコントローラーを使って移動処理を実行する。 */
			m_player->GetCharacterController().Execute(m_moveSpeed, MOVE_FRAME_SPEED);

			/* 計算結果の安全な座標をプレイヤー本体に設定する。 */
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());

			/* 着地時などにキャラコンが計算したY軸の速度を変数に反映する。 */
			m_jumpVelocity = m_moveSpeed.y;
		}


		bool PlayerJumpState::RequestID(uint8_t& id)
		{
			/* 着地したらIdleに戻る。*/
			if (m_player->GetCharacterController().IsOnGround())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enIdle);
				return true;
			}


			return false;
		}
	}
}