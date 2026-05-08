#include "stdafx.h"
#include "Src/Effect/EffectList.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Gun/Parameter/BulletParameterTable.h"

namespace nsApp
{
	void EffectListener::OnBulletFired(const BulletFireRequest& request)
	{
		/* 早期リターン。*/
		if (!m_target)
			return;

		if (!m_soundManager) {
			m_soundManager = FindGO<nsSound::SoundLister>("SoundManager");
		}

		/* Jobの情報を取得。*/
		m_currentJob = m_target->GetCurrentWeapon();
		auto& effectList = m_target->GetEffectList();

		/* 銃口の位置を計算する。*/
		const auto& param = BulletParameterTable::GetParameter(request.kind);
		m_gunMuzzlePosition = request.basePosition;
		m_gunMuzzlePosition.y += param.spawnOffsetY;

		/* 銃口の向きを計算する。*/
		m_gunMuzzleDirection = request.direction;
		m_gunMuzzleDirection.Normalize();
		m_gunMuzzlePosition += m_gunMuzzleDirection * param.spawnForwardOffset;

		m_currentJob = m_target->GetCurrentWeapon();


		/* 種類に応じて再生するエフェクトを決める。*/
		switch (m_currentJob)
		{
		case WeaponType::GreatSword:
			effectList.PlayEffect(nsEffect::Effect_ID::Charge, request.basePosition);
			break;

		case WeaponType::Hammer:
			effectList.PlayEffect(nsEffect::Effect_ID::Fire, request.basePosition);
			break;

		case WeaponType::Wand:
			effectList.PlayEffect(nsEffect::Effect_ID::MagicAttack, request.basePosition);
			break;

		case WeaponType::TwinGun:
			effectList.PlayEffect(nsEffect::Effect_ID::Shot, m_gunMuzzlePosition,Quaternion::Identity ,Vector3::One * 20.0f, 0.1f);
			break;

		default:
			break;
		}

		if (m_soundManager)
			m_soundManager->GetSEList().PlayAttackSE(m_currentJob, AttackType::NormalAttack);
	}
}