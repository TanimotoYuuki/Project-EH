#pragma once

/**
 * @file   ParameterAsyncLoadTask.h
 * @brief パラメータの非同期ロードタスク。
 * @author Yamaguchi Hayato
 * @date   2026/06/03: クラス作成日。
 */

#include "AsyncLoadTaskBase.h"
#include "Src/Parameter/ParameterSystem.h"


namespace nsApp
{
	class ParameterAsyncLoadTask : public AsyncLoadTaskBase
	{
	public:
		/* コンストラクタとデストラクタ。*/
		ParameterAsyncLoadTask() = default;
		virtual ~ParameterAsyncLoadTask() = default;


	protected:
		/**
		 * @brief ワーカースレッドでパラメーターを読み込む。
		 * @return 読み込みに成功した場合はtrue。
		 */
		bool LoadOnWorkerThread() override;

		/**
		 * @brief メインスレッドでの確定処理。
		 * @return 確定処理に成功した場合はtrue。
		 */
		bool FinalizeOnMainThread() override;

		/**
		 * @brief アニメ先読みはファイル数が多いので重めにする。
		 */
		float GetWeight() const override
		{
			return 1.0f;
		}

	private:
		ParameterSystem m_parameterSystem; //! パラメーターシステム。
	};
}
