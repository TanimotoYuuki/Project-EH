#pragma once

namespace nsApp 
{
	namespace nsNPC
	{
		class IThreatZone
		{
		public:
			/* デストラクタ。*/
			virtual ~IThreatZone() = default;


		public:
			/**
			 * @brief 指定位置の危険度（0 = 安全、大きいほど危険）。
			 * @param position 座標。
			 * @return　危険度（0 = 安全、大きいほど危険）。
			 */
			virtual float GetDangerCost(const Vector3& position) const = 0;

			/**
			 * @brief ゾーンが有効かどうか。
			 * @return 有効な場合は true、無効な場合は false。
			 */
			virtual bool IsActive() const = 0;
		};
	}
}