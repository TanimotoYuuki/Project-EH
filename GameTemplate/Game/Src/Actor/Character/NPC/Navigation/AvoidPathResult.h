#pragma once

/* 
 * @file     AvoidPathResult.h
 * @brief    回避移動の結果を表す構造体。
 * @author   Yamaguchi Hayato
 * @date     2026/06/21: クラス作成。
 * @note     Pathfinder が返し、State は direction だけ使う。
 * @details  回避移動の結果を表す構造体。Pathfinder が返し、State は direction だけ使う。
 */

namespace nsApp
{
	namespace nsNPC
	{
		/**
		 * @struct AvoidPathResult
		 * @brief 回避移動の結果。Pathfinder が返し、State は direction だけ使う。
		 */
		struct AvoidPathResult
		{
			Vector3 direction = Vector3::Zero; //! 移動方向（XZ、未正規化可）。
			bool hasValidDirection = false;    //! 有効な回避方向が見つかったか。
		};
	}
}