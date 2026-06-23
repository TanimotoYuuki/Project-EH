#include "stdafx.h"
#include "BossMeleeThreatProvider.h"
#include "Src/Actor/Character/Boss/Boss.h"

namespace
{
	const float THREAT_MARGIN = 70.0f;			//! ヒット半径に足す余白。NPC が早めに避けるため。
	const float BITE_DANGER_WEIGHT = 1.0f;
	const float TAIL_DANGER_WEIGHT = 1.0f;
}

namespace nsApp
{
	namespace nsNPC
	{
		void BossMeleeThreatProvider::CollectThreats(std::vector<IThreatZone*>& outZones)
		{
			/* 毎フレームいったん無効化してから、有効なものだけ登録する。*/
			m_biteZone.SetActive(false);
			m_tailZone.SetActive(false);

			/* ボスが存在しない場合は何もしない。*/
			auto* boss = FindGO<nsActor::Boss>("boss");
			if (boss == nullptr)
				return;

			/* ボスが攻撃ステートのときだけ近接危険を登録する。*/
			if (boss->GetCurrentStateID() != nsActor::BossStateID::enAttack)
				return;

			/* ボスの WeaponHitDetection を ThreatZoneCircle に同期する。*/
			SyncHitZone(boss->GetBiteHit(), m_biteZone, BITE_DANGER_WEIGHT);
			SyncHitZone(boss->GetTailHit(), m_tailZone, TAIL_DANGER_WEIGHT);

			/* 有効な危険ゾーンだけを outZones に追加する。*/
			if (m_biteZone.IsActive())
				outZones.push_back(&m_biteZone);
			if (m_tailZone.IsActive())
				outZones.push_back(&m_tailZone);
		}



		void BossMeleeThreatProvider::SyncHitZone(WeaponHitDetection& hit, ThreatZoneCircle& zone, float dangerWeight)
		{
			/* ヒット判定が無効なら危険ゾーンも無効化する。*/
			if (!hit.IsActive())
			{
				/* ヒット判定が無効なら危険ゾーンも無効化する。*/
				zone.SetActive(false);
				return;
			}

			/* ヒット判定が有効なら危険ゾーンを更新する。*/
			zone.SetCenter(hit.GetPosition());

			/* ヒット判定の半径に余白を足す。NPC が早めに避けるため。*/
			zone.SetRadius(hit.GetRadius() + THREAT_MARGIN);

			/* 危険度の重みを設定する。*/
			zone.SetDangerWeight(dangerWeight);

			/* 危険ゾーンを有効化する。*/
			zone.SetActive(true);
		}
	}
}