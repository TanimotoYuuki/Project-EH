#include "stdafx.h"
#include "CharacterAsyncLoadTask.h"
#include "Src/Actor/Character/Common/CharacterLoadContext.h"

namespace nsApp
{
	bool CharacterAsyncLoadTask::LoadOnWorkerThread()
	{
		/* 進捗を初期化する。*/
		SetProgress(0.0f);

		/* 前回の先読み結果を破棄する。*/
		CharacterLoadContext::Reset();

		/* ワーカーでキャラ／ボス／武器モデルを先読みする。*/
		SetProgress(0.3f);
		CharacterLoadContext::PrepareOnWorker(m_request);

		/* 先読み結果を確認する。*/
		const bool isSuccess = CharacterLoadContext::IsReady();
		if (isSuccess)
			SetProgress(1.0f);

		return isSuccess;
	}


	bool CharacterAsyncLoadTask::FinalizeOnMainThread()
	{
		/* Worker で Load した TkmFile をバンクへ登録する。*/
		/* 以降の Player / Boss の Init(path) は再読込せずバンク命中する。*/
		CharacterLoadContext::RegisterToBankOnMain();
		return CharacterLoadContext::IsReady();
	}
}