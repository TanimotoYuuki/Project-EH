#include "stdafx.h"
#include "EffectAsyncLoadTask.h"
#include "Src/Effect/EffectList.h"

namespace nsApp
{
	bool EffectAsyncLoadTask::LoadOnWorkerThread()
	{
		/* Effekseer は Main 専用なので、Worker では進捗だけ進める。*/
		SetProgress(0.0f);
		SetProgress(1.0f);
		return true;
	}


	bool EffectAsyncLoadTask::FinalizeOnMainThread()
	{
		/* EffectList の登録表を唯一のソースにして、EffectEngine へ載せる。*/
		nsEffect::EffectList effectList;
		effectList.Init();
		return true;
	}
}