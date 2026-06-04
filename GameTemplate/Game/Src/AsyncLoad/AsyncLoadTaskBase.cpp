#include "stdafx.h"
#include "AsyncLoadTaskBase.h"
#include <chrono>
#include <exception>

namespace nsApp
{
	AsyncLoadTaskBase::~AsyncLoadTaskBase()
	{
		/*タスクがまだ実行中の場合は、完了を待つ。*/
		Wait();
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

		/* エラーメッセージを初期化する。*/
		SetErrorMessage("");

		/* ワーカースレッドでロード処理を開始する。*/
		m_future = std::async(std::launch::async, [this]()
			{
				/* ワーカースレッドでロード処理を実行する。*/
				return LoadOnWorkerThread();
			});

		return true;
	}


	void AsyncLoadTaskBase::Update()
	{
		if (m_state == EnState::enLoading)
		{
			/* ワーカースレッドの処理が完了しているかを確認する。*/
			auto status = m_future.wait_for(std::chrono::milliseconds(0));

			/* ワーカースレッドの処理がまだ完了していない場合は、更新を続ける。*/
			if (status != std::future_status::ready)
				return;

			/* ワーカースレッドの処理が完了した場合は、結果を取得する。*/
			m_isSuccess = false;

			/* ワーカースレッドの処理結果を取得する際に例外が発生する可能性があるため、try-catchブロックで囲む。*/
			try
			{
				/* ワーカースレッドの処理結果を取得する。*/
				m_isSuccess = m_future.get();
			}
			catch (const std::exception& e)
			{
				SetErrorMessage(e.what());
				m_state = EnState::enFailed;
				return;
			}
			catch (...)
			{
				SetErrorMessage("Unknown async load error.");
				m_state = EnState::enFailed;
				return;
			}

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
			/* メインスレッドでの最終処理を実行する。*/
			m_isSuccess = FinalizeOnMainThread();

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