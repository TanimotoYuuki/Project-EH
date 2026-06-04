#include "stdafx.h"
#include "ParameterAsyncLoadTask.h"

namespace
{
	const auto PROGRESS_RATE = 1.0f; //! 進捗率。
}

namespace nsApp
{
	bool ParameterAsyncLoadTask::LoadOnWorkerThread()
	{
		/* パラメーターを読み込む。*/
		SetProgress(0.0f);

		/* パラメーターの読み込みに成功した場合は、進捗率を1.0fに設定してtrueを返す。*/
		bool isSuccess = m_parameterSystem.LoadAll();

		/* パラメーターの読み込みに成功した場合は、進捗率を1.0fに設定する。*/
		if (isSuccess)
			SetProgress(PROGRESS_RATE);

		return isSuccess;
	}


	bool ParameterAsyncLoadTask::FinalizeOnMainThread()
	{
		return true;
	}
}