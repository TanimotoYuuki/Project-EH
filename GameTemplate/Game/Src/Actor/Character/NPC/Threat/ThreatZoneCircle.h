#pragma once

#include "IThreatZone.h"

namespace nsApp
{
	namespace nsNPC
	{
		class ThreatZoneCircle : public IThreatZone
		{
		/* セッター。*/
		public:
			/**
			 * @brief Active状態をセットする。
			 * @param active 有効かどうか。
			 */
			inline void SetActive(bool active)
			{
				m_isActive = active;
			}

			/**
			 * @brief 危険度をセットする。
			 * @param center 危険度。
			 */
			inline void SetCenter(const Vector3& center)
			{
				m_center = center;
				m_center.y = 0.0f; //! Y軸は無視する。
			}

			/**
			 * @brief 半径をセットする。
			 * @param radius 半径。
			 */
			inline void SetRadius(float radius)
			{
				m_radius = radius;
			}

			/**
			 * @brief 危険度の重みをセットする。
			 *        Pathfinder が同じ距離でも「より避けるべき攻撃」を区別するために使う。
			 * @param weight 重み（1.0 が標準）。
			 */
			inline void SetDangerWeight(float weight)
			{
				m_dangerWeight = weight;
			}


		/* ゲッター。*/
		public:
			/* 継承枠。*/

			/**
			 * @brief 指定位置の危険度（0 = 安全、大きいほど危険）。
			 * @param position 座標。
			 * @return　危険度（0 = 安全、大きいほど危険）。
			 */ 
			float GetDangerCost(const Vector3& position) const override;

			/**
			 * @brief ゾーンが有効かどうか。
			 * @return 有効な場合は true、無効な場合は false。
			 */
			inline bool IsActive() const override
			{
				return m_isActive;
			}


		private:
			Vector3 m_center = Vector3::Zero;	//! 危険度の中心座標。

			float m_radius = 0.0f;				//! 危険度の半径。
			float m_dangerWeight = 1.0f;		//! 危険度の重み。

			bool m_isActive = false;			//! ゾーンが有効かどうか。
		};
	}
}
