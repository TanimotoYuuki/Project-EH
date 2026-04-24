#include "stdafx.h"
#include "PlayerMagicAttackState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerMagicAttackState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃の種類をセット。*/
			m_currentAttackType = AttackType::ChargeAttack;

			/* アニメーションを再生。*/
			m_player->PlayWeaponAnimation(AttackType::ChargeAttack);
		}


		void PlayerMagicAttackState::Update()
		{
			/* 親クラスの更新。*/
			PlayerAttackBaseState::Update();

			/* nフレーム目で魔法を発射する。*/
			if (m_attackTimer == 15)
			{
				m_spawnPosition = m_player->GetPosition();
				m_spawnPosition.y += 10.0f;
				m_spawnPosition += m_player->GetForwardVector() * 10.0f;

				/* */
				m_missileMagic = NewGO<nsActor::MagicProjectotile>(0, "ChargeMagic");

				/* ターゲットを探してセットする。*/
				m_missileMagic->SetTarget(m_player->SearchCharacter());

				/* 魔法の種類をセット。*/
				m_missileMagic->Initialize(nsActor::MagicType::enChargeMagic, m_spawnPosition, m_player->GetForwardVector());
			}

			/* アニメーションが終わったら待機状態へ戻る */
			if (m_attackTimer > 15 && !m_player->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
			}
		}


	}
}