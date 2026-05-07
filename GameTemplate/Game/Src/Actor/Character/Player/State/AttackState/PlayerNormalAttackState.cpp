#include "stdafx.h"
#include "PlayerNormalAttackState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Gun/Bullet/NormalBullet.h"

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

			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerNormalAttackState::Update()
		{
			if (!m_player)
				return;

			if (m_attackTimer == 48)
			{
				/* WandCharacterを選択時、通常攻撃の際、ミサイルを飛ばす処理。*/
				if(m_player->GetCurrentWeapon() == WeaponType::Wand)
					SummonMissile();

			    else if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				    FireGunBullet();
			}
			
			/* 更新作業。*/
			PlayerAttackBaseState::Update();
		}


		bool PlayerNormalAttackState::RequestID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enNormalAttack, id);
		}


		void PlayerNormalAttackState::SummonMissile()
		{
			if(m_player->GetCurrentWeapon() == WeaponType::Wand)
			{
				m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();
				m_spawnPosition.y += 10.0f;
				m_spawnPosition += m_player->GetForwardVector() * 10.0f;

				/* ミサイルを生成する。*/
				auto* normalMagicMissile = NewGO<nsActor::MagicProjectotile>(0, "NormalMagic");
				normalMagicMissile->Initialize(nsActor::MagicType::enNormalMagic, m_spawnPosition, m_player->GetForwardVector());
			}
		}


		void PlayerNormalAttackState::FireGunBullet()
		{
			m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();
			m_spawnPosition.y += 10.0f;

			auto* bullet = NewGO<nsActor::NormalBullet>(0, "NormalBullet");
			bullet->InitializeBullet(m_spawnPosition, m_player->GetForwardVector(), 4.0f, 60);
		}
	}
}