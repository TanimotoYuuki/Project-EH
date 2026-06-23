#pragma once

/**
 * @file   SimpleAvoidPathfinder.h
 * @brief  危険ゾーンを避ける移動方向をサンプリングで求める。
 * @author Yamaguchi Hayato
 * @date   2026/06/21: クラス作成
 * @note   危険ゾーンを避ける移動方向をサンプリングで求める。Brain から委譲して呼ぶ。
 */

#include "AvoidPathResult.h"
#include "Src/Actor/Character/NPC/Threat/IThreatZone.h"
#include "Src/Actor/Character/NPC/Social/NPCSocialContext.h"
#include <vector>

namespace nsApp
{
	namespace nsNPC
	{
		/**
		 * @class SimpleAvoidPathfinder
		 * @brief 候補方向を数本試し、危険度が最も低い方向を返す。
		 *
		 * @note  状態を持たない static ユーティリティ。Brain から委譲して呼ぶ。
		 */
		class SimpleAvoidPathfinder
		{
		public:
			/**
			 * @brief 回避方向を計算する。
			 * @param currentPosition NPC の現在位置。
			 * @param zones           今フレーム有効な危険ゾーン。
			 */
			static AvoidPathResult Compute(const Vector3& currentPosition, const std::vector<IThreatZone*>& zones, int partyIndex, const NPCSocialContext& socialContext);
		};
	}
}