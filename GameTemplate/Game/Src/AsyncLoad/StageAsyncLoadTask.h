#pragma once

/**
 * @file   StageAsyncLoadTask.h
 * @brief  ステージモデルの非同期先読みタスク。
 * @note   Worker で tkm を先読みする。Main の Initialize は InGameBuild 側のまま。
 */

#include "AsyncLoadTaskBase.h"

namespace nsApp
{
	/**
	 * @class StageAsyncLoadTask
	 * @brief ステージモデルを JobQueue Worker 経由で先読みするタスク。
	 */
	class StageAsyncLoadTask : public AsyncLoadTaskBase
	{
	public:
		/* コンストラクタとデストラクタ。*/
		StageAsyncLoadTask() = default;
		virtual ~StageAsyncLoadTask() = default;


	protected:
		/**
		 * @brief ワーカースレッドでステージモデルを先読みする。
		 * @return 先読みに成功した場合は true。
		 */
		bool LoadOnWorkerThread() override;

		/**
		 * @brief メインスレッドでの確定処理。
		 * @return 確定処理に成功した場合は true。
		 * @note   第1段では追加処理なし（BuildStage が従来どおり Initialize する）。
		 */
		bool FinalizeOnMainThread() override;
	};
}