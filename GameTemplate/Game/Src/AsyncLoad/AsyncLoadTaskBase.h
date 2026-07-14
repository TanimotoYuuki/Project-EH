#pragma once

/**
 * @file   AsyncLoadTaskBase.h
 * @brief  非同期ロードタスクの基底クラス。
 * @author YamaguchiHayato
 * @date   2026/06/03
 */

#include <atomic>
#include <cstdint>
#include <string>
#include <mutex>


namespace nsApp
{
	/**
	 * @brief 非同期ロードタスクの状態。
	 * - enIdle: タスクが開始されていない状態。
	 * - enLoading: タスクが実行中の状態。
	 * - enFinalize: タスクが完了し、結果の処理が必要な状態。
	 * - enCompleted: タスクが正常に完了した状態。
	 * - enFailed: タスクが失敗した状態。
	 */
	enum class EnState : uint8_t
	{
		enIdle,			//! < タスクが開始されていない状態。
		enLoading,		//! < タスクが実行中の状態。
		enFinalize,		//! < タスクが完了し、結果の処理が必要な状態。
		enCompleted,    //! < タスクが正常に完了した状態。
		enFailed,	    //! < タスクが失敗した状態。
		enInitialize,   //! 初期化用。
	};

	class AsyncLoadTaskBase
	{
	public:
		/* コンストラクタとデストラクタ。*/
		AsyncLoadTaskBase() = default;
		virtual ~AsyncLoadTaskBase();

		/* コピー&ムーブの禁止。*/
		AsyncLoadTaskBase(const AsyncLoadTaskBase&) = delete;
		AsyncLoadTaskBase& operator = (const AsyncLoadTaskBase&) = delete;


	public:
		/**
		 * @brief 非同期ロードの開始処理。
		 */
		bool Start();

		/**
		 * @brief 非同期ロードの状態を更新する。
		 */
		void Update();

		/**
		 * @brief 非同期ロードの完了を待つ。
		 * @note  DF と同じく Job ID で完了待ちする。実装は .cpp。
		 */
		void Wait();


	public:
		/**
		 * @brief ロードが完了しているか。
		 */
		inline bool IsCompleted() const
		{
			return m_state == EnState::enCompleted;
		}

		/**
		 * @brief ロードに失敗しているか。
		 * @return 失敗している場合はtrue。
		 */
		bool IsFailed() const
		{
			return m_state == EnState::enFailed;
		}

		/**
		 * @brief ロード中か。
		 * @return ロード中の場合はtrue。
		 */
		inline bool IsRunning() const
		{
			return m_state == EnState::enLoading || m_state == EnState::enFinalize;
		}

		/**
		 * @brief ロード処理が終了しているか。
		 * @return 成功または失敗で終了している場合はtrue。
		 */
		inline bool IsFinished() const
		{
			return m_state == EnState::enCompleted || m_state == EnState::enFailed;
		}

		/**
		 * @brief 進捗率を取得する。
		 * @return 0.0f ～ 1.0f の進捗率。
		 */
		inline float GetProgress() const
		{
			return static_cast<float>(m_progressRate.load() / 1000.0f);
		}

		/**
		 * @brief 現在の状態を取得する。
		 * @return 現在の状態。
		 */
		inline EnState GetState() const
		{
			return m_state;
		}

		/**
		 * @brief エラーメッセージを取得する。
		 * @return エラーメッセージ。
		 */
		inline std::string GetErrorMessage() const
		{
			std::lock_guard<std::mutex> lock(m_errorMutex);
			return m_errorMessage;
		}


	protected:
		/**
		 * @brief ワーカースレッドで実行するロード処理。
		 * @return ロードに成功した場合はtrue。
		 */
		virtual bool LoadOnWorkerThread() = 0;

		/**
		 * @brief メインスレッドで実行する確定処理。
		 * @return 確定処理に成功した場合はtrue。
		 */
		virtual bool FinalizeOnMainThread()
		{
			return true;
		}

		/**
		 * @brief 進捗率を設定する。
		 * @param progress 0.0f ～ 1.0f の進捗率。
		 */
		inline void SetProgress(float progress)
		{
			m_clampProgress = Clamp01(progress);
			m_progressRate = static_cast<int>(m_clampProgress * 1000.0f);
		}

		/**
		 * @brief エラーメッセージを設定する。
		 * @param errorMessage エラーメッセージ。
		 */
		inline void SetErrorMessage(const std::string& errorMessage)
		{
			std::lock_guard<std::mutex> lock(m_errorMutex);
			m_errorMessage = errorMessage;
		}


	private:
		/**
		 * @brief 値を0.0f ～ 1.0fに収める。
		 * @param value 対象の値。
		 * @return 0.0f ～ 1.0fに収めた値。
		 */
		float Clamp01(float value) const;


	private:
		uint64_t m_workerJobId = 0;					   //! ワーカー Job の ID（DF と同様。0 なら無効）。
		std::atomic<int> m_progressRate{ 0 };		   //! 進捗率を整数で表現するための変数。0 ～ 100の範囲で使用。
		std::string m_errorMessage;					   //! ロード中のエラーメッセージ。

		EnState m_state = EnState::enInitialize;	   //! タスクの現在の状態。     

		mutable std::mutex m_errorMutex;			   //! エラーメッセージへのアクセスを保護するためのミューテックス。

		float m_clampProgress = 0.0f;				   //! 進捗率を0.0f ～ 1.0fに収めるための値。

		bool m_isSuccess = false;					   //! ロード処理の成功フラグ。
	};
}