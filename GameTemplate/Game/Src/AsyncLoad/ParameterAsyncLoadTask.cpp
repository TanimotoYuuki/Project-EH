#include "stdafx.h"
#include "ParameterAsyncLoadTask.h"

namespace nsApp
{
	bool ParameterAsyncLoadTask::LoadOnWorkerThread()
	{
		/* パラメーターを読み込む。*/
		SetProgress(0.0f);

		/* テーブルごとに進捗を更新しながら読み込む。*/
		bool isSuccess = m_parameterSystem.LoadAll(
			[this](float progress)
			{
				SetProgress(progress);
			});

		return isSuccess;
	}


	bool ParameterAsyncLoadTask::FinalizeOnMainThread()
	{
		return true;
	}
}