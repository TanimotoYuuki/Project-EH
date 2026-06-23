#pragma once

/**
 * @file   BossMeleeThreatProvider.h
 * @brief  ボス近接攻撃（噛み・尻尾）のヒット判定を Threat 層へ公開する。
 */

#include "IThreatProvider.h"
#include "ThreatZoneCircle.h"
#include "Src/Actor/Character/Common/WeaponHitDetection.h"

namespace nsApp
{
	namespace nsNPC
	{
		/**
		 * @class BossMeleeThreatProvider
		 * @brief Boss の WeaponHitDetection（噛み・尻尾）を IThreatZone に変換する。
		 *
		 * @note  近接は同時に最大 2 つ（噛み / 尻尾）なので、メンバー 2 個で足りる。
		 *        火球・竜巻などは別 Provider で扱う。
		 */
		class BossMeleeThreatProvider : public IThreatProvider
		{
		public:
			/**
			 * @brief 有効な近接ヒット判定を outZones に追加する。
			 * @param outZones ThreatCollector が保持する危険ゾーン一覧。
			 */
			void CollectThreats(std::vector<IThreatZone*>& outZones) override;


		private:
			/**
			 * @brief WeaponHitDetection の状態を ThreatZoneCircle に同期する。
			 * @param hit           ボス側の当たり判定。
			 * @param zone          書き込み先の危険ゾーン。
			 * @param dangerWeight  この攻撃の危険度倍率。
			 */
			void SyncHitZone(WeaponHitDetection& hit, ThreatZoneCircle& zone, float dangerWeight);


		private:
			ThreatZoneCircle m_biteZone; //! 噛みつき攻撃の危険ゾーン。
			ThreatZoneCircle m_tailZone; //! 尻尾攻撃の危険ゾーン。
		};
	}
}