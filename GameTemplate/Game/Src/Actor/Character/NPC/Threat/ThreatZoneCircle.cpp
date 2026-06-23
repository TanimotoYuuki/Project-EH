#include "stdafx.h"
#include "ThreatZoneCircle.h"

namespace
{
	const float MIN_RADIUS = 0.001f;
}


namespace nsApp
{
	namespace nsNPC
	{
		float ThreatZoneCircle::GetDangerCost(const Vector3& position) const
		{
			/* ゾーンが無効、または半径が小さすぎる場合は危険度0を返す。*/
			if(!m_isActive || m_radius <= MIN_RADIUS)
				return 0.0f;

			/* Y軸は無視する。*/
			Vector3 diff = position - m_center;
			diff.y = 0.0f;

			/* 危険度を計算する。*/
			const float distance = diff.Length();
			if(distance >= m_radius)
				return 0.0f;

			/* 危険度は、中心に近いほど高くなるようにする。*/
			const float t = 1.0f - (distance / m_radius);
			return t * m_dangerWeight;
		}
	}
}