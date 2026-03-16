#include "stdafx.h"
#include "PlayerHitState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace
{
	const auto GRAVITY = 30.0f;                /* ジャンプと同じ重力をかける。*/
	const auto MOVE_FRAME_RATE = 1.0f / 60.0f; /* 1フレームあたりの固定時間。*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerHitState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 最初は吹っ飛びアニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Hit_Fly);

			/* ダメージを受けてからの時間をリセット。*/
			SetHitTimer(0);

			/* 起き上がるフラグをリセット。*/
			SetGetUpFlag(false);

			/* 吹っ飛ぶ初速を設定。*/
			SetKnockBackVelocity(400.0f);

			/* 吹っ飛ぶ速度を設定。*/ 
			SetKnockBackSpeed(Vector3(-100.0f, 50.0f, 0.0f));
		}


		void PlayerHitState::Update()
		{
			/* タイマーを加算する。*/
			m_hitTimer++;

			/* 吹っ飛び中の移動計算。*/
			if (!m_isGetUP)
			{
				/* x軸を毎フレーム少しずつ減らす。*/
				m_knockBackSpeed.x *= 0.9f;

				/* ジャンプと同じ重力をかける*/
				/* @TODO: リファ。*/
				m_knockBackVelocity -= GRAVITY;

				/* ステージを貫通しないように制限をかける。*/
				if (m_knockBackSpeed.y < -0.0)
					m_knockBackSpeed.y = 0.0f;

				/* 着地時のY軸の初速を反映させる。*/
				m_knockBackSpeed.y = m_knockBackVelocity;

				/* キャラクターコントローラーを用い、当たり判定の移動を行う。*/
				m_player->GetCharacterController().Execute(m_knockBackSpeed, MOVE_FRAME_RATE);
                /* 座標の更新。*/
				m_player->SetPosition(m_player->GetCharacterController().GetPosition());

				/* 着地時のY軸の初速を反映させる。*/
				m_knockBackVelocity = m_knockBackSpeed.y;
			}


			/* アニメーションの終了判定(時間経過)。*/
			if (m_hitTimer > 5)
			{
				/* アニメーションが再生し終わったら。*/
				if (!m_player->IsPlayAnimation())
				{
					if (!m_isGetUP)
					{
						/* 起き上がりアニメーションを再生。*/
						m_player->PlayBasicAnimation(CharacterBasicAnimationList::Hit_UP);

						/* 起き上がるフラグを立てる。*/
						SetGetUpFlag(true);

						/* タイマーをリセットする。*/
						SetHitTimer(0);
					}

					/* 起き上がりアニメーションが終わっていたら待機状態に戻る。*/
					else if (m_isGetUP)
						m_stateMachine->ChangeState(new PlayerIdleState());
				}
			}
		}


		bool PlayerHitState::RequestID(uint8_t& id)
		{
			/* 状態遷移。*/
			return false;
		}
	}
}