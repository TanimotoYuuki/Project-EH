#include "stdafx.h"

#include "Src/AsyncLoad/JobQueue.h"

namespace nsApp
{
    namespace nsJob
    {
        JobQueue::~JobQueue()
        {
			/* デストラクタでワーカースレッドを停止する。*/
            Shutdown();
        }


        JobQueue& JobQueue::GetInstance()
        {
            /* インスタンスを返す。*/
            static JobQueue instance;
            return instance;
        }


        void JobQueue::Startup(int workerCount)
        {
            /* すでに起動済みなら何もしない。*/
            if (isRunning_)
                return;

            /* ワーカー数が 1 未満なら 1 にする。*/
            if (workerCount < 1)
                workerCount = 1;

            /* ワーカー数が 8 より大きい場合は 8 にする。*/
            if(workerCount > 8)
				workerCount = 8;

            isRunning_ = true;

            /* ワーカースレッドを作成する。*/
            workers_.clear();
            workers_.reserve(static_cast<size_t>(workerCount));

            /* ワーカースレッドを起動する。*/
            for (int i = 0; i < workerCount; ++i)
                workers_.emplace_back([this]() { WorkerLoop(); });
        }


        void JobQueue::Shutdown()
        {
            /* すでに停止済みなら何もしない。*/
            if (!isRunning_)
                return;

            /* ワーカースレッドを停止する。*/
            isRunning_ = false;
            workerCv_.notify_all();

            /* ワーカースレッドの終了を待つ。*/
            for (auto& t : workers_)
            {
                /* スレッドが joinable なら join する。*/
                if (t.joinable())
                    t.join();
            }

            /* ワーカースレッドをクリアする。*/
            workers_.clear();

            {
                /* キューをクリアする。*/
                std::lock_guard<std::mutex> lock(workerMutex_);

                /* キューを空にする。*/
                while (!workerQueue_.empty())
                    workerQueue_.pop();
            }
            {
                /* キューをクリアする。*/
                std::lock_guard<std::mutex> lock(mainMutex_);

                /* キューを空にする。*/
                while (!mainQueue_.empty())
                    mainQueue_.pop();
            }
            {
                /* 完了フラグをクリアする。*/
                std::lock_guard<std::mutex> lock(doneMutex_);
                doneFlags_.clear();
            }

            /* カウンタをリセットする。*/
            activeWorkerJobs_ = 0;
            activeMainJobs_ = 0;
        }


        JobHandle JobQueue::EnqueueWorker(JobFn work)
        {
            /* Job ID を発行する。*/
            const uint64_t id = IssueId();

            {
                /* ワーカースレッド用キューに Job を追加する。*/
                std::lock_guard<std::mutex> lock(workerMutex_);
                workerQueue_.push(JobItem{id, std::move(work)});
            }

            /* ワーカースレッドに通知する。*/
            ++activeWorkerJobs_;
            workerCv_.notify_one();
            return JobHandle(id);
        }


        JobHandle JobQueue::EnqueueMain(JobFn work)
        {
            /* Job ID を発行する。*/
            const uint64_t id = IssueId();

            {
                /* メインスレッド用キューに Job を追加する。*/
                std::lock_guard<std::mutex> lock(mainMutex_);
                mainQueue_.push(JobItem{id, std::move(work)});
            }

            /* メインスレッドに通知する。*/
            ++activeMainJobs_;
            return JobHandle(id);
        }


        bool JobQueue::IsJobDone(uint64_t jobId) const
        {
            if (jobId == 0)
                return true;

            return IsDoneInternal(jobId);
        }


        void JobQueue::PumpMain()
        {
            /* メインスレッド用キューの Job を処理する。*/
            constexpr int kMaxJobsPerFrame = 8;

            /* 最大 kMaxJobsPerFrame 件の Job を処理する。*/
            for (int i = 0; i < kMaxJobsPerFrame; ++i)
            {
                /* メインスレッド用キューから Job を取得する。*/
                JobItem item;
                {
                    /* メインスレッド用キューの排他制御。*/
                    std::lock_guard<std::mutex> lock(mainMutex_);
                    if (mainQueue_.empty())
                        break;

                    /* メインスレッド用キューから Job を取得する。*/
                    item = std::move(mainQueue_.front());
                    mainQueue_.pop();
                }

                /* Job を実行する。*/
                if (item.work)
                    item.work();

                /* Job を完了済みにする。*/
                MarkDone(item.id);
                --activeMainJobs_;
            }
        }


        int JobQueue::GetActiveJobCount() const
        {
            /* ワーカースレッドとメインスレッドの未完了 Job 数を返す。*/
            return activeWorkerJobs_.load() + activeMainJobs_.load();
        }


        bool JobHandle::IsDone() const
        {
            /* id_ が 0 の場合は完了済みとみなす。*/
            if (id_ == 0)
                return true;

            /* JobQueue に完了状態を問い合わせる。*/
            return JobQueue::GetInstance().IsDoneInternal(id_);
        }


        void JobHandle::Wait() const
        {
            /* id_ が 0 の場合は完了済みとみなす。*/
            if (id_ == 0)
                return;

            /* JobQueue に完了するまで待つように問い合わせる。*/
            JobQueue::GetInstance().WaitInternal(id_);
        }


        void JobQueue::WorkerLoop()
        {
            /* ワーカースレッドのループ処理。*/
            while (isRunning_)
            {
                /* ワーカースレッド用キューから Job を取得する。*/
                JobItem item;
                {
                    /* ワーカースレッド用キューの排他制御。*/
                    std::unique_lock<std::mutex> lock(workerMutex_);
                    workerCv_.wait(lock, [this]() { return !isRunning_ || !workerQueue_.empty(); });

                    /* ワーカースレッドが停止中ならループを抜ける。*/
                    if (!isRunning_)
                        break;

                    /* ワーカースレッド用キューが空ならループを続ける。*/
                    if (workerQueue_.empty())
                        continue;

                    /* ワーカースレッド用キューから Job を取得する。*/
                    item = std::move(workerQueue_.front());
                    workerQueue_.pop();
                }

                /* Job を実行する。*/
                if (item.work)
                    item.work();

                /* Job を完了済みにする。*/
                MarkDone(item.id);
                --activeWorkerJobs_;
            }
        }


        uint64_t JobQueue::IssueId()
        {
            /* 新しい Job ID を発行する。*/
            const uint64_t id = nextId_.fetch_add(1);

            /* Job ID の完了状態を管理するためのフラグを作成する。*/
            auto flag = std::make_shared<std::atomic<bool>>(false);
            {
                /* 完了フラグの排他制御。*/
                std::lock_guard<std::mutex> lock(doneMutex_);
                doneFlags_[id] = flag;
            }
            return id;
        }


        void JobQueue::MarkDone(uint64_t id)
        {
            /* Job ID を完了済みにする。*/
            std::lock_guard<std::mutex> lock(doneMutex_);

            /* 完了フラグを true にする。*/
            auto it = doneFlags_.find(id);
            if (it != doneFlags_.end())
                doneFlags_.erase(it);
        }


        bool JobQueue::IsDoneInternal(uint64_t id) const
        {
            /* Job ID が完了済みかを確認する。*/
            std::lock_guard<std::mutex> lock(doneMutex_);

            /* 完了フラグを確認する（エントリが無い＝完了済み扱い）。*/
            auto it = doneFlags_.find(id);
            if (it == doneFlags_.end())
                return true;
            return it->second->load();
        }


        void JobQueue::WaitInternal(uint64_t id) const
        {
            for (;;)
            {
                /* Job ID が完了済みかを確認する。*/
                if (IsDoneInternal(id))
                    return;
                std::this_thread::yield();
            }
        }
    } // namespace nsJob
} // namespace nsApp
