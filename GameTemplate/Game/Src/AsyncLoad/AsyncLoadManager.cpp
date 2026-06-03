#include "stdafx.h"
#include "AsyncLoadManager.h"

namespace nsApp
{
	AsyncLoadManager::~AsyncLoadManager()
	{
		/* 全てのタスクをクリア。*/
		Clear();
	}


	void AsyncLoadManager::AddTask(std::unique_ptr<AsyncLoadTaskBase> task)
	{
		/* タスクが無いなら処理をしない。*/
		if (task == nullptr)
			return;

		/* ロード開始後はタスクを追加しない。*/
		if (m_isStarted)
			return;

		/* タスクをタスクリストに追加する。*/
		m_taskList.push_back(std::move(task));
	}


	void AsyncLoadManager::Start()
	{
		/* ロードがすでに開始されている場合は、開始できない。*/
		if (m_isStarted)
			return;

		/* ロード開始フラグを立てる。*/
		m_isStarted = true;

		/* タスクリスト内の全てのタスクを開始する。*/
		for (auto& task : m_taskList)
		{
			/* タスクが無いなら処理をしない。*/
			if (task == nullptr)
				continue;

			/* タスクを開始する。*/
			task->Start();
		}
	}


	void AsyncLoadManager::Update()
	{
		/* ロードが開始されていない場合は、更新しない。*/
		if (!m_isStarted)
			return;

		/* タスクリスト内の全てのタスクを更新する。*/
		for (auto& task : m_taskList)
		{
			/* タスクが無いなら処理をしない。*/
			if (task == nullptr)
				continue;

			/* タスクを更新する。*/
			task->Update();
		}
	}


	bool AsyncLoadManager::IsCompleted() const
	{
		/* タスクリストが空の場合は、全てのタスクが完了しているとみなす。*/
		if (m_taskList.empty())
			return false;

		/* タスクリスト内の全てのタスクが完了しているかを確認する。*/
		for (const auto& task : m_taskList)
		{
			/* タスクが無いなら処理をしない。*/
			if (task == nullptr)
				continue;

			/* タスクが完了していない場合は、全てのタスクが完了していないとみなす。*/
			if(!task->IsCompleted())
				return false;
		}

		return true;
	}	


	bool AsyncLoadManager::IsFailed() const
	{
		/* タスクリストが空の場合は、失敗しているタスクがないとみなす。*/
		for(const auto& task : m_taskList)
		{
			/* タスクが無いなら処理をしない。*/
			if (task == nullptr)
				continue;

			/* タスクが失敗している場合は、失敗しているタスクがあるとみなす。*/
			if(task->IsFailed())
				return true;
		}

		/* 失敗しているタスクがない場合は、失敗しているタスクがないとみなす。*/
		return false;
	}


	bool AsyncLoadManager::IsRunning() const
	{
		/* ロードが開始されていない場合は、ロード中ではないとみなす。*/
		if(!m_isStarted)
			return false;

		/* タスクリストが空の場合は、ロード中ではないとみなす。*/
		if (IsCompleted())
			return false;

		/* いずれかのタスクが失敗している場合は、ロード中ではないとみなす。*/
		if(IsFailed())
			return false;

		return true;
	}


	float AsyncLoadManager::GetProgress() const
	{
		/* タスクリストが空の場合は、進捗率を0.0fとみなす。*/
		if (m_taskList.empty())
			return 0.0f;

		/* タスクリスト内の全てのタスクの進捗率を合計する。*/
		float totalProgress = 0.0f;
		int validTaskCount = 0;

		/* タスクリスト内の全てのタスクの進捗率を合計する。*/
		for (const auto& task : m_taskList)
		{
			/* タスクが無いなら処理をしない。*/
			if (task == nullptr)
				continue;

			/* タスクの進捗率を合計する。*/
			totalProgress += task->GetProgress();
			++validTaskCount;
		}

		/* 有効なタスクがない場合は、進捗率を0.0fとみなす。*/
		if(validTaskCount <= 0)
			return 0.0f;

		/* タスクの平均進捗率を計算して返す。*/
		return totalProgress / static_cast<float>(validTaskCount);
	}


	std::string AsyncLoadManager::GetErrorMessage() const
	{
		/* タスクリストが空の場合は、エラーメッセージを空文字列とみなす。*/
		for (const auto& task : m_taskList)
		{
			/* タスクが無いなら処理をしない。*/
			if (task == nullptr)
				continue;

			/* タスクが失敗している場合は、そのタスクのエラーメッセージを返す。*/
			if (task->IsFailed())
				return task->GetErrorMessage();
		}

		return "";
	}
	

	void AsyncLoadManager::WaitAll()
	{
		/* タスクリスト内の全てのタスクが完了するまで待機する。*/
		for (const auto& task : m_taskList)
		{
			/* タスクが無いなら処理をしない。*/
			if (task == nullptr)
				continue;

			/* タスクが完了するまで待機する。*/
			task->Wait();
		}
	}
}