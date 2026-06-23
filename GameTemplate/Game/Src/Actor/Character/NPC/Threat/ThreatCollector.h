#pragma once

/**
 * @file   ThreatCollector.h
 * @brief  複数の IThreatProvider から危険ゾーンを集約し、NPC 回避判定に提供する。
 * @author Yamaguchi Hayato
 * @date   2024/06/21: 新規作成
 */

#include "IThreatZone.h"
#include "IThreatProvider.h"
#include <vector>
#include <memory>

namespace nsApp
{
	namespace nsNPC
	{
		/**
		 * @class ThreatCollector
		 * @brief 登録された Provider から毎フレーム危険ゾーンを収集する。
		 *
		 * @note  Provider は ThreatCollector の寿命より長く生きること（Brain が所有）。
		 *        Phase 2 で Fire / Tornado Provider を追加しても NPC 側は変更不要。
		 */
		class ThreatCollector
		{
		public:
			/**
			 * @brief Provider を登録する。Init 時に 1 回だけ呼ぶ。
			 * @param provider 危険源を提供するオブジェクト（ThreatCollector は所有しない）。
			 */
			void RegisterProvider(IThreatProvider* provider);

			/**
			 * @brief 全 Provider から危険ゾーンを再収集する。毎フレーム Update 冒頭で呼ぶ。
			 */
			void Collect();

			/**
			 * @brief 指定位置の合計危険度を返す。
			 * @param position 判定位置（通常は NPC の XZ 座標）。
			 * @return 全アクティブゾーンの危険度合計。
			 */
			float GetTotalDangerCost(const Vector3& position) const;

			/**
			 * @brief 危険度が閾値以上かどうか（回避すべきか）。
			 * @param position 判定位置。
			 * @param threshold  回避開始の閾値（例: 0.3f）。
			 */
			inline bool IsThreatened(const Vector3& position, float threshold) const
			{
				return GetTotalDangerCost(position) >= threshold;
			}

			/**
			 * @brief 現在フレームで収集されたアクティブゾーン一覧。
			 */
			inline const std::vector<IThreatZone*>& GetActiveZones() const
			{
				return m_activeZones;
			}


		private:
			std::vector<IThreatProvider*> m_providers; //! 登録済み Provider（非所有）。
			std::vector<IThreatZone*> m_activeZones;   //! 今フレーム有効なゾーンへのポインタ。
		};
	}
}