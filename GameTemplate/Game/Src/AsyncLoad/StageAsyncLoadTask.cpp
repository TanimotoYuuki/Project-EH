#include "stdafx.h"
#include "StageAsyncLoadTask.h"
#include "Src/Actor/Stage/StageLoadContext.h"

namespace nsApp
{
	bool StageAsyncLoadTask::LoadOnWorkerThread()
	{
		/* 進捗を初期化する。*/
		SetProgress(0.0f);

		/* 前回の先読み結果を破棄する。*/
		nsStage::StageLoadContext::Reset();

		/* ワーカーでステージ1モデルを先読みする。*/
		SetProgress(0.3f);
		nsStage::StageLoadContext::PrepareOnWorker(nsStage::StageID::stage1);

		/* 先読み結果を確認する。*/
		const bool isSuccess = nsStage::StageLoadContext::IsReady();
		if (isSuccess)
			SetProgress(1.0f);

		return isSuccess;
	}


	bool StageAsyncLoadTask::FinalizeOnMainThread()
	{
		/* 第1段では Main 側の追加確定処理は行わない。*/
		return true;
	}
}