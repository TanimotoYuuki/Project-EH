#pragma once

/**
 * @file   BossFireThreatProvider.h
 * @brief  ボス火炎攻撃（火球・着弾炎柱・Rain）を Threat 層へ公開する。
 */

#include "IThreatProvider.h"
#include "ThreatZoneCircle.h"
#include "Src/Actor/Character/Common/WeaponHitDetection.h"

namespace nsApp
{
	namespace nsNPC
	{
		/**
		 * @class BossFireThreatProvider
		 * @brief Boss の火炎系 WeaponHitDetection / Rain 滴を IThreatZone に変換する。
		 */
		class BossFireThreatProvider : public IThreatProvider
		{
		public:
			/**
			 * @brief IThreatProvider の実装。Boss の火炎系攻撃を IThreatZone として出力する。
			 * @param outZones 出力先の IThreatZone 配列。Boss の火球・着弾炎柱・Rain 滴を ThreatZoneCircle として追加する。
			 */
			void CollectThreats(std::vector<IThreatZone*>& outZones) override;


		private:
			/**
			 * @brief WeaponHitDetection の情報を ThreatZoneCircle に同期する。攻撃が有効な場合は ThreatZoneCircle をアクティブにし、位置と半径を設定する。
			 * @param hit WeaponHitDetection のインスタンス。攻撃が有効かどうか、位置を取得するために使用する。
			 * @param zone 同期先の ThreatZoneCircle。攻撃が有効な場合はアクティブにし、位置と半径を設定する。
			 * @param dangerWeight この攻撃の危険度を表す重み。ThreatZoneCircle の危険度として設定される。
			 */
			void SyncHitZone(WeaponHitDetection& hit, ThreatZoneCircle& zone, float dangerWeight);

			/**
			 * @brief Rain Drop の情報を ThreatZoneCircle に同期する。攻撃が有効な場合は ThreatZoneCircle をアクティブにし、位置と半径を設定する。
			 * @param position Rain Drop の位置。Y 成分は無視され、ThreatZoneCircle の中心の Y は 0 に固定される。
			 * @param radius Rain Drop の当たり判定半径。
			 * @param zone 同期先の ThreatZoneCircle。攻撃が有効な場合はアクティブにし、位置と半径を設定する。
			 * @param dangerWeight この攻撃の危険度を表す重み。ThreatZoneCircle の危険度として設定される。
			 */
			void SyncRainDropZone(const Vector3& position, float radius, ThreatZoneCircle& zone, float dangerWeight);


		private:
			static const int MAX_RAIN_ZONES = 16;		  //! BossFireCombatSystem::MAX_RAIN_DROPS と同期。

			ThreatZoneCircle m_fireballZone;              //! 飛翔中火球。
			ThreatZoneCircle m_landingZone;               //! 着弾炎柱。
			ThreatZoneCircle m_rainZones[MAX_RAIN_ZONES]; //! Rain 滴（最大16）。
		};
	}
}