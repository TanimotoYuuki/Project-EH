#pragma once

/**
 * @file   BattleCameraStrategy.h
 * @brief  戦闘中のカメラを担当するカメラ。
 * @autor  Yamaguchi Hayato
 * @date   2026/06/08: クラス作成日。
 * @detail StrategyPatternで設計。
 */

#include "Src/Camera/Camera.h"
#include "Src/Camera/ICameraStrategy.h"

namespace nsApp
{
	class BattleCameraStrategy : public ICameraStrategy
	{
	public:
		/* コンストラクタとデストラクタ。*/
		BattleCameraStrategy() = default;
		virtual ~BattleCameraStrategy() = default;


	public:
		/**
		 * @brief 更新処理。
		 */
		void Update() override;
	};
}