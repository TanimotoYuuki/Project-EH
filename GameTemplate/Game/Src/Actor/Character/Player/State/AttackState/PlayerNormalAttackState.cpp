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

			/* 攻撃タイプを保存する。*/
			m_currentAttackType = AttackType::NormalAttack;

			/* 攻撃アニメーションを再生する。*/ 
			m_player->PlayWeaponAnimation(AttackType::NormalAttack);

			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerNormalAttackState::Update()
		{
			if (!m_player)
				return;

			/* 更新作業。*/
			PlayerAttackBaseState::Update();
		}


		bool PlayerNormalAttackState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enNormalAttack, id);
		}
	}
}