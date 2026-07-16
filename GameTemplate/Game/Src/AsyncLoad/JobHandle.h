#pragma once

#include <atomic>
#include <cstdint>

namespace nsApp
{
    namespace nsJob
    {
        /**
         * @class JobHandle
         * @brief 非同期 Job の完了状態を表すハンドル。
         */
        class JobHandle
        {
        public:
            /* コンストラクタ。*/
            JobHandle() : id_(0) {}

            /* コピームーブを禁止する。*/
            JobHandle(const JobHandle&) = default;
            JobHandle& operator=(const JobHandle&) = default;


             /**
             * @brief JobHandle をリセットする。
              */
             void Reset() { id_ = 0; }

            /**
             * @brief 有効なハンドルか。
             * @return id が 0 でなければ true。
             */
            bool IsValid() const { return id_ != 0; }

            /**
             * @brief Job が完了したか。
             * @return 完了していれば true。
             */
            bool IsDone() const;

            /**
             * @brief 完了するまでブロックする（デバッグ用。ゲームループ内では非推奨）。
             */
            void Wait() const;

            /**
             * @brief JobHandle の ID を取得する。
             * @return JobHandle の ID。
             */
            inline uint64_t GetId() const
            {
                return id_;
            }


        private:
            friend class JobQueue;

            /**
             * @brief JobHandle の ID を設定する。
             * @param id JobHandle の ID。
             */
            explicit JobHandle(uint64_t id) : id_(id) {}

            uint64_t id_ = 0;
        };
    } // namespace nsJob
} // namespace nsApp
