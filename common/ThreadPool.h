/*****************************************************************************
 * @file: ThreadPool.cpp
 * @brief: Based on ThreadPool @ https://github.com/progschj/ThreadPool.git
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
 ****************************************************************************/

#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <chrono>

#include "Global.h"

namespace cppbase {

enum class ThreadPriority
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
int32_t GetThreadPriority(int64_t id);

class DLLEXPORT ThreadPool
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
    ThreadPool(uint32_t threads, ThreadPriority priority, uint32_t cpu_reserved = 0);
    ~ThreadPool();

    template<class F, class... Args>
    auto Enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

    ThreadPriority GetThreadPriority() const;
    uint32_t GetReservedCpu() const;

private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > m_threads;
    // the task queue
    std::queue< std::function<void()> > m_tasks;

    // synchronization
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    bool m_stop{false};
    ThreadPriority m_priority;
    uint32_t m_cpu_reserved{0};
};

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(m_stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        m_tasks.emplace([task](){
            using namespace std::chrono;
            auto start = high_resolution_clock::now();
            (*task)();
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            // olog_infod("ThreadPool", "Thread pool task execution time: {} us", duration.count());
        });
    }
    m_condition.notify_one();
    return res;
}

} // namespace cppbase

#include "ThreadPool_intl.h"
