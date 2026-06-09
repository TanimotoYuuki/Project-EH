#pragma once

/**
 * @file   AsyncLoadManager.h
 * @brief  非同期ロードを管理する。
 * @author Yamaguchi Hayato
 * @date   2026/06/03: クラス作成日。
 */

#include <vector>
#include <memory>
#include <string>
#include "AsyncLoadTaskBase.h"


namespace nsApp
{
	class AsyncLoadManager
	{
	public:
		/* コンストラクタとデストラクタ。*/
		AsyncLoadManager() = default;
		virtual ~AsyncLoadManager();


	public:
		/**
		 * @brief 非同期ロードタスクを追加する。
		 * @param tast 追加する非同期ロードタスク。
		               タスクはAsyncLoadManagerが所有するため、呼び出し元はタスクの管理を行う必要がない。
		 */
		void AddTask(std::unique_ptr<AsyncLoadTaskBase> tast);

		/**
		 * @brief 追加タスクを開始する。
		 */
		void Start();

		/**
		 * @brief ぎ同期タスクの状態を更新する。
		 */
		void Update();

		/**
		 * @brief すべてのタスクをクリアする。
		 */
		inline void Clear()
		{
			/* タスクリストをクリアする。*/
			WaitAll(); 

			m_taskList.clear();
			m_isStarted = false;
		}


	public:
		/**
		 * @brief すべてのタスクが完了しているか。
		 * @return すべてのタスクが成功または失敗で完了している場合はtrue。
			       まだ実行中のタスクがある場合はfalse。
			       タスクが1つもない場合はtrue。
		 */
		bool IsCompleted() const;

		/**
		 * @brief いずれかのロードが失敗しているか。
		 * @return 失敗しているタスクがある場合はtrue。
		 */
		bool IsFailed() const;

		/**
		 * @brief ロード中か。
		 * @return ロード中の場合はtrue。
		 */
		bool IsRunning() const;

		/**
		 * @brief 全体の進捗率を取得する。
		 * @return 0.0f ～ 1.0f の進捗率。
		 */
		float GetProgress() const;

		/**
		 * @brief エラーメッセージを取得する。
		 * @return エラーメッセージ。
		 */
		std::string GetErrorMessage() const;


	private:
		/**
		 * @brief 全タスクの完了を待つ。
		 */
		void WaitAll();


	private:
		std::vector<std::unique_ptr<AsyncLoadTaskBase>> m_taskList; //! 非同期ロードタスクのリスト。
		bool m_isStarted = false;								    //! ロード開始済みか。
	};
}
