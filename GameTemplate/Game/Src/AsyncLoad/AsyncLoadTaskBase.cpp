#include "stdafx.h"
#include "AsyncLoadTaskBase.h"
#include "Src/AsyncLoad/JobQueue.h"
#include <exception>
#include <thread>

namespace nsApp
{
	AsyncLoadTaskBase::~AsyncLoadTaskBase()
	{
		/*タスクがまだ実行中の場合は、完了を待つ。*/
		Wait();
	}


	void AsyncLoadTaskBase::Wait()
	{
		/* JobQueue のインスタンスを取得する。*/
		auto& jobQuece = nsJob::JobQueue::GetInstance();

		/* メインスレッド Job の完了を待つ。*/
		if (m_workerJobId != 0)
		{
			/* メインスレッド Job の完了を待つ。*/
			while (!jobQuece.IsJobDone(m_workerJobId))
				std::this_thread::yield();

			/* Job ID をリセットする。*/
			m_workerJobId = 0;
		}

		/* ワーカースレッド Job の完了を待つ。*/
		if (m_mainJobId != 0)
		{
			/* ワーカースレッド Job の完了を待つ。*/
			while (!jobQuece.IsJobDone(m_mainJobId))
			{
				jobQuece.PumpMain();
				std::this_thread::yield();
			}

			/* Job ID をリセットする。*/
			m_mainJobId = 0;
		}
	}


	bool AsyncLoadTaskBase::Start()
	{
		/* タスクがすでに実行中の場合は、開始できない。*/
		if (m_state == EnState::enLoading)
			return false;

		/* メインスレッド確定処理中の場合も、開始できない。*/
		if (m_state == EnState::enFinalize)
			return false;

		/* タスクの状態をローディングに設定する。*/
		m_state = EnState::enLoading;

		/* 進捗率を初期化する。*/
		m_progressRate = 0;

		/* 成功フラグを初期化する。*/
		m_isSuccess = false;

		/* Job ID を初期化する。*/
		m_mainJobId = 0;
		m_isMainFinalizeEnqueued = false;

		/* エラーメッセージを初期化する。*/
		SetErrorMessage("");

		/* ワーカースレッドでロード処理を開始する。*/
		m_workerJobId =
			nsJob::JobQueue::GetInstance()
			.EnqueueWorker(
				[this]()
				{
					/* ワーカースレッドの処理結果を取得する際に例外が発生する可能性があるため、try-catchブロックで囲む。*/
					try
					{
						/* ワーカースレッドでロード処理を実行する。*/
						m_isSuccess = LoadOnWorkerThread();
					}
					catch (const std::exception& e)
					{
						SetErrorMessage(e.what());
						m_isSuccess = false;
					}
					catch (...)
					{
						SetErrorMessage("Unknown async load error.");
						m_isSuccess = false;
					}
				})
			.GetId();

		return m_workerJobId != 0;
	}


	void AsyncLoadTaskBase::Update()
	{
		if (m_state == EnState::enLoading)
		{
			/* ワーカースレッドの処理が完了しているかを確認する。*/
			if (!nsJob::JobQueue::GetInstance().IsJobDone(m_workerJobId))
				return;

			/* Job ID をリセットする。*/
			m_workerJobId = 0;

			/* ワーカースレッドの処理が完了した場合は、結果を確認する。*/
			if (!m_isSuccess)
			{
				if (GetErrorMessage().empty())
					SetErrorMessage("Async load failed.");

				m_state = EnState::enFailed;
				return;
			}

			m_state = EnState::enFinalize;
		}


		if (m_state == EnState::enFinalize)
		{
			auto& jobQueue = nsJob::JobQueue::GetInstance();

			/* Phase 1: Finalize を Main Job として 1 回だけ積む。*/
			if (!m_isMainFinalizeEnqueued)
			{
				m_isMainFinalizeEnqueued = true;
				m_mainJobId = jobQueue
					.EnqueueMain(
						[this]()
						{
							/* メインスレッドでの最終処理を実行する。*/
							m_isSuccess = FinalizeOnMainThread();
						})
					.GetId();
				return;
			}

			/* Phase 2: Main Job 完了を待つ。*/
			if (!jobQueue.IsJobDone(m_mainJobId))
				return;

			m_mainJobId = 0;

			if (!m_isSuccess)
			{
				/* エラーメッセージをセット。*/
				if (GetErrorMessage().empty())
					SetErrorMessage("Async load finalization failed.");

				/* ロード失敗状態に遷移する。*/
				m_state = EnState::enFailed;
				return;
			}

			/* ロード成功状態に遷移する。*/
			SetProgress(1.0f);
			m_state = EnState::enCompleted;
		}
	}


	float AsyncLoadTaskBase::Clamp01(float value) const
	{
		/* 値を0.0f ～ 1.0fに収める。*/
		if (value < 0.0f)
			return 0.0f;
		if (value > 1.0f)
			return 1.0f;

		/* 補正値を戻り値とする。*/
		return value;
	}
}