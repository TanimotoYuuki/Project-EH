#include "stdafx.h"
#include "PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"

#include "Src/Actor/Gun/Bullet/IGunBullet.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace
{
	const auto ATTACK_FRAME_15 = 15;		   //! 攻撃の15フレーム目。
	const auto WEAPON_ROTATION_ANGLE = -90.0f; //! 武器の回転角度。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerRushEndState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定する。*/
			SetCurrentAttackType(AttackType::RushAttack_End);

			/* 再生するアニメーションをセット。*/
			m_player->PlayWeaponAnimation(AttackType::RushAttack_End);
		}


		void PlayerRushEndState::OnAttackTick()
		{
			/* 15フレーム目に処理。*/
			if (m_attackTimer == ATTACK_FRAME_15)
			{
				/* 銃の場合。*/
				m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();
				/* 銃の前方を弾の進行方向とする。*/
				m_forwardDirection = m_player->GetForwardVector();
				/* 乱射攻撃の弾を生成して発射する。*/
				ConstructAndTransmitBulletRequest(BulletType::enRush);
			}
		}


		bool PlayerRushEndState::OnUpdateAttack()
		{
			/* アニメーションの再生終了を待つ。*/
			if (!m_player->IsPlayAnimation())
			{
				/* 銃の場合。*/
				if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
				{
					/* サブウェポンをリセットする。*/
					m_player->ResetSubWeapon();
					/* 武器の角度をリセットする。*/
					m_player->SetWeaponRotationAngle(Vector3::Front, WEAPON_ROTATION_ANGLE);
				}

				/* 乱射攻撃のフィニッシュとしてチャージ攻撃を放つ。*/
				m_stateMachine->ChangeState(new PlayerChargeAttackState());
				return true;
			}

			return false;
		}
	}
}