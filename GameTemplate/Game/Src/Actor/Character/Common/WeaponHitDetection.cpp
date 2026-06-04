#include "stdafx.h"
#include "WeaponHitDetection.h"

namespace
{
	const auto HIT_RADIUS = 30.0f; //! 武器の当たり判定の半径。
}


namespace nsApp
{
	void WeaponHitDetection::Init(float radius)
	{
		/* 当たり判定をセットする。*/
		m_radius = radius;
		/* 合ったり判定をセットする。*/
		SetRadius(m_radius);
	}


	bool WeaponHitDetection::IsHit(nsActor::ICharacter* target)
	{
		/* 武器が有効でない、もしくは敵がいない場合は当たらない。*/
		if(!m_isActive || !target)
			return false;

		/* すでに斬ったリストの敵がいるかチェック。*/
		for (auto hitTarget : m_hitTargets)
		{
			if (hitTarget == target)
				return false;
		}

		/* 敵の座標と武器の座標を図る。*/
		m_targetPosition = target->GetPosition();
		m_diffVector = m_position - m_targetPosition;

		/* 距離を計る。*/
		m_diff = m_diffVector.Length();

		if (m_diff < (m_radius + 30.0f))
		{
			/* 当たった敵をリストに登録。*/
			m_hitTargets.push_back(target);
			return true;
		}

		return false;
	}
}