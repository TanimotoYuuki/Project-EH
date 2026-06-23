#pragma once

#include "NPCSocialContext.h"

namespace nsApp
{
	namespace nsNPC
	{
		/**
		 * @class NPCSocialEvaluator
		 * @brief 味方との距離に基づく位置ペナルティを計算する。
		 */
		class NPCSocialEvaluator
		{
		public:
			/**
			 * @brief サンプル位置の Social ペナルティ（大きいほど避けたい）。
			 * @param samplePosition 評価する位置。
			 * @param context          自分自身など。
			 */
			static float CalcPositionPenalty(const Vector3& samplePosition, const NPCSocialContext& context);


			/**
			 * @brief 味方から離れるための分離ベクトルを返す（未正規化可、無ければ Zero）。
			 * @param myPosition 自分の現在位置。
			 * @param context      自分自身など。
			 * @return 分離方向の単位ベクトル。不要なら Zero。
			 */
			static Vector3 CalcSeparationVector(const Vector3& myPosition, const NPCSocialContext& context);
		};
	}
}