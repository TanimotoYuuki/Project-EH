#include "stdafx.h"
#include "AnimationAsyncLoadTask.h"
#include "Src/Actor/Character/Common/AnimationLoadContext.h"

namespace nsApp
{
	bool AnimationAsyncLoadTask::LoadOnWorkerThread()
	{
		/* 進捗を初期化する。*/
		SetProgress(0.0f);

		/* 前回の先読み結果を破棄する。*/
		AnimationLoadContext::Reset();

		/* ワーカーでキャラ／ボスアニメを先読みする。*/
		SetProgress(0.3f);
		AnimationLoadContext::PrepareOnWorker(m_request);

		/* 先読み結果を確認する。*/
		const bool isSuccess = AnimationLoadContext::IsReady();
		if (isSuccess)
			SetProgress(1.0f);

		return isSuccess;
	}


	bool AnimationAsyncLoadTask::FinalizeOnMainThread()
	{
		/* Worker で Load した TkaFile をバンクへ登録する。*/
		/* 以降の AnimationClip::Load は再読込せずバンク命中する。*/
		AnimationLoadContext::RegisterToBankOnMain();
		return AnimationLoadContext::IsReady();
	}
}