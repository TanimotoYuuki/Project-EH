#include "stdafx.h"
#include "PlayerNormalAttackState.h"

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
	}
}