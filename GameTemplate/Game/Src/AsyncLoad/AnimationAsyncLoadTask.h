#pragma once

/**
 * @file   AnimationAsyncLoadTask.h
 * @brief  キャラ／ボスアニメ（.tka）の非同期先読みタスク。
 * @note   Worker で TkaFile::Load、Main でバンク登録する。
 *         AnimationClip::Load は InGameBuild 側のまま（バンク命中）。
 */

#include "AsyncLoadTaskBase.h"
#include "Src/Build/InGameBuildHelper.h"

namespace nsApp
{
	/**
	 * @class AnimationAsyncLoadTask
	 * @brief アニメーションを JobQueue Worker 経由で先読みするタスク。
	 */
	class AnimationAsyncLoadTask : public AsyncLoadTaskBase
	{
	public:
		/**
		 * @brief コンストラクタ。
		 * @param request InGame 構築リクエスト（役割・ボスタイプ）。
		 */
		explicit AnimationAsyncLoadTask(const InGameBuildRequest& request)
			: m_request(request)
		{
		}

		virtual ~AnimationAsyncLoadTask() = default;


	protected:
		/**
		 * @brief ワーカースレッドでアニメーションを先読みする。
		 * @return 先読みに成功した場合は true。
		 */
		bool LoadOnWorkerThread() override;

		/**
		 * @brief メインスレッドでバンク登録する。
		 * @return 確定処理に成功した場合は true。
		 */
		bool FinalizeOnMainThread() override;

		/**
		 * @brief アニメ先読みはファイル数が多いので重めにする。
		 */
		float GetWeight() const override
		{
			return 4.0f;
		}

	private:
		InGameBuildRequest m_request;	//! 先読み対象を決めるリクエスト。
	};
}