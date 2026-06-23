#include "stdafx.h"
#include "WeaponHitDetection.h"

namespace
{
	const auto DEFAULT_HIT_RADIUS = 30.0f;      //! デフォルトの当たり判定半径。
	const auto TARGET_HIT_PADDING = 30.0f;    //! ターゲット側の当たり判定余白。
}

namespace nsApp
{
	void WeaponHitDetection::Init(float radius)
	{
		m_radius = radius;
		SetRadius(m_radius);
	}


	bool WeaponHitDetection::IsHit(nsActor::ICharacter* target)
	{
		/* 判定が無効、またはターゲットがいない場合はヒットしない。*/
		if (!m_isActive || target == nullptr)
			return false;

		/* 既にヒット済みリストに入っているターゲットかチェック。*/
		for (auto* hitTarget : m_hitTargets)
		{
			if (hitTarget == target)
				return false;
		}

		/* ターゲットの座標と武器の座標の差分を求める。*/
		m_targetPosition = target->GetPosition();
		m_diffVector = m_position - m_targetPosition;

		m_diff = m_diffVector.Length();

		if (m_diff < (m_radius + TARGET_HIT_PADDING))
		{
			/* ヒットしたターゲットをリストに登録。*/
			m_hitTargets.push_back(target);
			return true;
		}

		return false;
	}
}