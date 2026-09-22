/*****************************************************************************
 * @file: ThreadPool.cpp
 * @brief: Based on ThreadPool @ https://github.com/progschj/ThreadPool.git
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 ****************************************************************************/

#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "Global.h"

namespace cppbase {

enum class ThreadPriority : int32_t
{
    IDLE = 10,
    LOW = 30,
    NORMAL = 50,
    HIGH = 70,
    CRITICAL = 90,
};

/**
 * @brief SetThreadPriority
 * @param priority
 */
int32_t SetThreadPriority(ThreadPriority priority);

/**
 * @brief GetThreadPriority
 * @param id
 */
int32_t GetThreadPriority(std::thread::native_handle_type id);

class ThreadPool
{
public:
    /**
     * @brief ThreadPool Construct a thread pool
     * @param threads Number of threads in the thread pool
     * @param priority Thread priority for individual thread
     * @param cpu_reserved Number of cpu core that should be reserved.
     * @note  If the value > 0, the thread affinity for the threads will be set.
     *        e.g. The CPU has 4 cores, and threads = 4, cpu_reserved = 1, then
     *        the thread affinity for the 4 threads in the thread pool will be
     *        set to CPU1, CPU2, CPU3, CPU1, and CPU0 is not used.
     */
    explicit ThreadPool(uint32_t threads, ThreadPriority priority, uint32_t cpu_reserved = 0);
    ThreadPool() = delete;
    ~ThreadPool();

    // Defined inline (rather than declared here and defined out-of-line below) because MSVC
    // fails to match a template member function's out-of-line definition to its declaration
    // when the trailing return type is a dependent std::invoke_result<...>::type (C2244).
    template <class F, class... Args>
    auto Enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>
    {
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()> >(
            [f = std::forward<F>(f), ... args = std::forward<Args>(args)]() mutable -> return_type {
                return std::invoke(std::move(f), std::move(args)...);
            });

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);

            // don't allow enqueueing after stopping the pool
            if (m_stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            m_tasks.emplace([task]() { (*task)(); });
        }
        m_condition.notify_one();
        return res;
    }

    ThreadPriority GetThreadPriority() const;
    uint32_t GetReservedCpu() const;

    /**
     * @brief QueueSize Number of tasks currently waiting to be picked up by a worker thread.
     */
    size_t QueueSize() const;

    /**
     * @brief ThreadCount Number of worker threads in the pool.
     */
    size_t ThreadCount() const;

private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> m_threads;
    // the task queue
    std::queue<std::function<void()> > m_tasks;

    // synchronization
    mutable std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    bool m_stop{false};
    ThreadPriority m_priority;
    uint32_t m_cpu_reserved{0};
};

}  // namespace cppbase

#include "ThreadPool_intl.h"
