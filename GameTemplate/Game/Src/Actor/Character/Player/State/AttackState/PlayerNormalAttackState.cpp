#include "stdafx.h"
#include "PlayerNormalAttackState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"
#include "Src/Actor/Magic/Factory/MagicFactory.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerNormalAttackState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定する。*/
			m_currentAttackType = AttackType::NormalAttack;

			/* 攻撃アニメーションを再生する。*/
			m_player->PlayWeaponAnimation(AttackType::NormalAttack);
		}


		void PlayerNormalAttackState::OnAttackTick()
		{
			if (m_attackTimer == 1)
				m_player->GetWeaponHitDetection().Enable();
		}


		bool PlayerNormalAttackState::OnUpdateAttack()
		{
			const bool isRangeWeapon =
				m_player->GetCurrentWeapon() == WeaponType::Wand ||
				m_player->GetCurrentWeapon() == WeaponType::TwinGun;

			if (!isRangeWeapon)
				return false;

			/* 48フレーム目で遠距離武器の弾丸を発射。*/
			if (m_attackTimer == 48)
			{
				/* 杖の場合。*/
				if (m_player->GetCurrentWeapon() == WeaponType::Wand)
					SummonMissile();

				/* 銃の場合。*/
				else if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
					FireGunBullet();
			}

			/* 遠距離武器は50Fまでは攻撃状態を維持する。*/
			if (m_attackTimer < 50)
				return true;

			return false;
		}


		bool PlayerNormalAttackState::OnRequestAttackID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enNormalAttack, id);
		}


		void PlayerNormalAttackState::SummonMissile()
		{
			if (m_player->GetCurrentWeapon() != WeaponType::Wand)
				return;

			/* 座標を取得。*/
			m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();

			/* 前方向のベクトルを取得する。*/
			m_forwardDirection = m_player->GetForwardVector();

			/* 魔法の生成。*/
			ConstructAndTransmitMagicRequest(nsActor::MagicType::enNormalMagic);
		}


		void PlayerNormalAttackState::FireGunBullet()
		{
			/* 座標を取得。*/
			m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();

			/* 前方向のベクトルを取得する。*/
			m_forwardDirection = m_player->GetForwardVector();

			/* 生成する弾丸の種類を設定する。*/
			ConstructAndTransmitBulletRequest(BulletType::enNormal);
		}
	}
}