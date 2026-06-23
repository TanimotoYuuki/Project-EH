#include "stdafx.h"
#include "BossFireThreatProvider.h"
#include "Src/Actor/Character/Boss/Boss.h"

namespace
{
	const float THREAT_MARGIN = 70.0f;
	const float FIREBALL_DANGER_WEIGHT = 1.0f;
	const float LANDING_DANGER_WEIGHT = 1.2f;
	const float RAIN_DANGER_WEIGHT = 1.0f;
}

namespace nsApp
{
	namespace nsNPC
	{
		void BossFireThreatProvider::CollectThreats(std::vector<IThreatZone*>& outZones)
		{
			// まず全ての ThreatZone を非アクティブにする。
			// Boss の攻撃状態に応じて必要なものだけアクティブにしていく。
			m_fireballZone.SetActive(false);
			m_landingZone.SetActive(false);

			/* Rain Drop 用の ThreatZoneCircle を全て非アクティブにする。*/
			for (int i = 0; i < MAX_RAIN_ZONES; ++i)
				m_rainZones[i].SetActive(false);

			/* ボスを探索。*/
			auto* boss = FindGO<nsActor::Boss>("boss");
			if (boss == nullptr)
				return;

			/* Boss の攻撃状態に応じて ThreatZone を同期していく。*/
			SyncHitZone(boss->GetFireHit(), m_fireballZone, FIREBALL_DANGER_WEIGHT);
			SyncHitZone(boss->GetFireLandingHit(), m_landingZone, LANDING_DANGER_WEIGHT);

			/* Rain Drop 用の ThreatZoneCircle を Boss の Rain Drop 状態に応じて同期していく。*/
			const int rainDropCount = boss->GetFireRainDropCount();
			const float rainRadius = boss->GetFireRainHitRadius();

			/* BossFireCombatSystem::MAX_RAIN_DROPS と同期すること。*/
			for (int i = 0; i < rainDropCount && i < MAX_RAIN_ZONES; ++i)
			{
				/* Rain Drop がアクティブでないなら ThreatZoneCircle も非アクティブのままスキップ。*/
				if (!boss->IsFireRainDropActive(i))
					continue;

				/* Rain Drop がアクティブなら ThreatZoneCircle を同期してアクティブにする。*/
				SyncRainDropZone(
					boss->GetFireRainDropPosition(i),
					rainRadius,
					m_rainZones[i],
					RAIN_DANGER_WEIGHT);
			}

			/* 最後に、アクティブな ThreatZoneCircle だけを outZones に追加していく。*/
			if (m_fireballZone.IsActive())
				outZones.push_back(&m_fireballZone);
			if (m_landingZone.IsActive())
				outZones.push_back(&m_landingZone);

			/* Rain Drop 用の ThreatZoneCircle を全て outZones に追加する。非アクティブなものは IsActive() で判定されるため、問題ない。*/
			for (int i = 0; i < MAX_RAIN_ZONES; ++i)
			{
				if (m_rainZones[i].IsActive())
					outZones.push_back(&m_rainZones[i]);
			}
		}


		void BossFireThreatProvider::SyncHitZone(WeaponHitDetection& hit, ThreatZoneCircle& zone, float dangerWeight)
		{
			/* ヒット判定が非アクティブなら ThreatZoneCircle も非アクティブにする。*/
			if (!hit.IsActive())
			{
				zone.SetActive(false);
				return;
			}

			/* ヒット判定がアクティブなら ThreatZoneCircle をヒット位置と半径に応じて同期してアクティブにする。*/
			zone.SetCenter(hit.GetPosition());
			zone.SetRadius(hit.GetRadius() + THREAT_MARGIN);
			zone.SetDangerWeight(dangerWeight);
			zone.SetActive(true);
		}


		void BossFireThreatProvider::SyncRainDropZone(const Vector3& position, float radius, ThreatZoneCircle& zone, float dangerWeight)
		{
			/* Rain Drop の位置と半径に応じて ThreatZoneCircle を同期してアクティブにする。*/
			zone.SetCenter(position);
			zone.SetRadius(radius + THREAT_MARGIN);
			zone.SetDangerWeight(dangerWeight);
			zone.SetActive(true);
		}
	}
}