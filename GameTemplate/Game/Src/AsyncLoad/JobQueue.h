#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <unordered_map>

#include "Src/AsyncLoad/JobHandle.h"

namespace nsApp
{
    namespace nsJob
    {
        /**
         * @class JobQueue
         * @brief ワーカースレッド用キューとメインスレッド用キューを管理する。
         * @note  ファイル I/O や TSV パースはワーカー、D3D/GPU 初期化はメインで実行する。
         */
        class JobQueue
        {
        public:
            friend class JobHandle;

            using JobFn = std::function<void()>;

            /**
			 * @brief デストラクタ。
             */
            virtual ~JobQueue();

            /**
			 * @brief シングルトンインスタンスを取得する。
			 * @return JobQueue のシングルトンインスタンス。
             */
            static JobQueue& GetInstance();

            /**
             * @brief ワーカースレッドを起動する。
             * @param workerCount ワーカー数（推奨 2）。
             */
            void Startup(int workerCount = 2);

            /**
			 * @brief ワーカースレッドを停止する。
			 * @note  Shutdown 後は再度 Startup できる。
             */
            void Shutdown();

            /**
             * @brief ワーカースレッドで実行する Job を登録する。
             * @param work CPU 処理（ファイル読み込み・パースなど）。
             * @return 完了監視用ハンドル。
             */
            JobHandle EnqueueWorker(JobFn work);

            /**
             * @brief メインスレッドで実行する Job を登録する。
             * @param work GPU 初期化や NewGO などメイン専用処理。
             * @return 完了監視用ハンドル。
             */
            JobHandle EnqueueMain(JobFn work);

            /**
             * @brief Job ID が完了したか。
             * @param jobId Enqueue が返した ID（0 なら完了扱い）。
             */
            bool IsJobDone(uint64_t jobId) const;

            /**
             * @brief メインスレッド用キューを1フレーム処理する。
             * @note  Game::Update の先頭で毎フレーム呼ぶ。
             */
            void PumpMain();

            /** @brief ワーカー＋メインの未完了 Job 数（ざっくり進捗用）。 */
            int GetActiveJobCount() const;


        private:
            /* コンストラクタとデストラクタ。*/
            JobQueue() = default;
            virtual ~JobQueue();

            /* コピームーブを禁止する。*/
            JobQueue(const JobQueue&) = delete;
            JobQueue& operator=(const JobQueue&) = delete;


        private:
            /**
             * @brief ワーカースレッドのループ処理。
             */
            void WorkerLoop();

            /**
             * @brief Job ID を発行する。
             * @return 新しい Job ID。
             */
            uint64_t IssueId();

            /**
             * @brief Job ID を完了済みにする。
             * @param id 完了させる Job ID。
             */
            void MarkDone(uint64_t id);

            /**
             * @brief Job ID が完了済みかを確認する。
             * @param id 確認する Job ID。
             * @return 完了済みなら true。
             */
            bool IsDoneInternal(uint64_t id) const;

            /**
             * @brief Job ID が完了するまでブロックする。
             * @param id ブロックする Job ID。
             */
            void WaitInternal(uint64_t id) const;


        private:
            /**
             * @struct JobItem 
             * @brief Job の情報を保持する構造体。
             */
            struct JobItem
            {
                uint64_t id = 0; //! JobHandle の ID。
                JobFn work;      //! Job の処理内容。
            };

            std::vector<std::thread> workers_;                                           //! ワーカースレッド群。
            std::queue<JobItem> workerQueue_;                                            //! ワーカースレッド用キュー。
            std::queue<JobItem> mainQueue_;                                              //! メインスレッド用キュー。

            mutable std::mutex workerMutex_;                                             //! ワーカースレッド用キューの排他制御。
            mutable std::mutex mainMutex_;                                               //! メインスレッド用キューの排他制御。
            mutable std::mutex stateMutex_;                                              //! JobQueue の状態（isRunning_）の排他制御。
            std::condition_variable workerCv_;                                           //! ワーカースレッド用キューの通知用条件変数。
             
            std::atomic<bool> isRunning_{false};                                         //! ワーカースレッドが稼働中か。
            std::atomic<int> activeWorkerJobs_{0};                                       //! ワーカースレッドの未完了 Job 数。
            std::atomic<int> activeMainJobs_{0};                                         //! メインスレッドの未完了 Job 数。
            std::atomic<uint64_t> nextId_{1};                                            //! JobHandle の次の ID。

            // id → 完了フラグ
            mutable std::mutex doneMutex_;                                               //! 完了フラグの排他制御。
            std::unordered_map<uint64_t, std::shared_ptr<std::atomic<bool>>> doneFlags_; //! JobHandle の完了フラグ。
        };
    } // namespace nsJob
} // namespace nsApp
