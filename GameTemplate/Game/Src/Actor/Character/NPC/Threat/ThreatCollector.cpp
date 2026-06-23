#include "stdafx.h"
#include "ThreatCollector.h"

namespace nsApp
{
	namespace nsNPC
	{
		void ThreatCollector::RegisterProvider(IThreatProvider* provider)
		{
			/* nullptr チェック。*/
			if (provider == nullptr)
				return;

			/* すでに登録済みかチェック。*/
			m_providers.push_back(provider);
		}


		void ThreatCollector::Collect()
		{
			/* 毎フレームいったんクリアしてから、Provider から危険ゾーンを収集する。*/
			m_activeZones.clear();

			/* Provider から危険ゾーンを収集する。*/
			for (IThreatProvider* provider : m_providers)
			{
				/* nullptr チェック。*/
				if (provider == nullptr)
					continue;

				/* Provider から危険ゾーンを収集する。*/
				provider->CollectThreats(m_activeZones);
			}
		}


		float ThreatCollector::GetTotalDangerCost(const Vector3& position) const
		{
			/* すべての有効な危険ゾーンの危険度を合計する。*/
			float total = 0.0f;

			/* 有効な危険ゾーンの危険度を合計する。*/
			for (IThreatZone* zone : m_activeZones)
			{
				/* nullptr チェック。*/
				if (zone == nullptr || !zone->IsActive())
					continue;

				/* 危険度を合計する。*/
				total += zone->GetDangerCost(position);
			}

			/* 合計値を返す。*/
			return total;
		}
	}
}