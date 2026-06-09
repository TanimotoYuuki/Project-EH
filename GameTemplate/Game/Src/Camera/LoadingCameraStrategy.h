#pragma once

/**
 * @file   LoadingCameraStrategy.h
 * @brief  ローディング中のカメラを担当するカメラ。
 * @autor  Yamaguchi Hayato
 * @date   2026/06/08: クラス作成日。
 * @detail StrategyPatternで設計。
 */

#include "Src/Camera/ICameraStrategy.h"

namespace nsApp
{
	class LoadingCameraStrategy :public ICameraStrategy
	{
	public:
		/* コンストラクタとデストラクタ。*/
		LoadingCameraStrategy() = default;
		virtual ~LoadingCameraStrategy() = default;


	public:
		/**
		 * @brief 更新処理。
		 */
		void Update() override;
	};
}
