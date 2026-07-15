#pragma once

/**
 * @file   CharacterAsyncLoadTask.h
 * @brief  キャラ／ボス／武器モデルの非同期先読みタスク。
 * @note   Worker で TkmFile::Load、Main でバンク登録する。
 *         Player / Boss の Init(path) は InGameBuild 側のまま。
 */

#include "AsyncLoadTaskBase.h"
#include "Src/Build/InGameBuildHelper.h"

namespace nsApp
{
	/**
	 * @class CharacterAsyncLoadTask
	 * @brief キャラ関連モデルを JobQueue Worker 経由で先読みするタスク。
	 */
	class CharacterAsyncLoadTask : public AsyncLoadTaskBase
	{
	public:
		/**
		 * @brief コンストラクタ。
		 * @param request InGame 構築リクエスト（役割・ボスタイプ）。
		 */
		explicit CharacterAsyncLoadTask(const InGameBuildRequest& request)
			: m_request(request)
		{
		}

		virtual ~CharacterAsyncLoadTask() = default;


	protected:
		/**
		 * @brief ワーカースレッドでキャラ関連モデルを先読みする。
		 * @return 先読みに成功した場合は true。
		 */
		bool LoadOnWorkerThread() override;

		/**
		 * @brief メインスレッドでバンク登録する。
		 * @return 確定処理に成功した場合は true。
		 */
		bool FinalizeOnMainThread() override;


	private:
		InGameBuildRequest m_request;	//! 先読み対象を決めるリクエスト。
	};
}