#include "stdafx.h"
#include "PlayerNormalAttackState.h"

namespace
{
	const auto RUSH_COUNT = 2; /* 連続攻撃に繋げるための連打数。*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerNormalAttackState::Enter()
		{
			/* 親クラスのEnterを呼び出してPlayerクラス本体にセットする。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃アニメーションを再生する。*/ 
			m_player->PlayWeaponAnimation(AttackType::NormalAttack);
		}


		void PlayerNormalAttackState::Update()
		{
			/* 更新作業。*/
			PlayerAttackBaseState::Update();
		}


		bool PlayerNormalAttackState::RequestID(uint8_t& id)
		{
			/* 1段目の攻撃で連打数をチェック。*/
			if (m_attackTimer > 15)
			{
				/* n回以上連打しているなら連続攻撃状態へ*/
				if (m_rushCount >= RUSH_COUNT)
				{
					id = static_cast<uint8_t>(nsActor::PlayerStateID::enRushStart);
					return true;
				}
			}
			return false;
		}
	}
}