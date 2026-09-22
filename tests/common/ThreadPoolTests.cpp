/**************************************************************************
 * @file:  ThreadPoolTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 **************************************************************************/

#include <common/ThreadPool.h>
#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#endif

using namespace cppbase;

TEST(ThreadPoolTests, SimpleTest)
{
    ThreadPool pool(4, ThreadPriority::NORMAL);
    std::vector<std::future<int32_t>> results;

    for (int32_t i = 0; i < 8; ++i)
    {
        results.emplace_back(pool.Enqueue([i] {
#ifdef __linux__
            std::cout << "ThreadPoolTests::SimpleTest thread=" << i
                      << " running on CPU=" << sched_getcpu() << std::endl;
#endif
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i * i;
        }));
    }

    int32_t ret = 0;
    for (auto&& result : results)
    {
        EXPECT_EQ(result.get(), ret * ret);
        ret++;
    }
}

TEST(ThreadPoolTests, PriorityAndCpuTest)
{
    auto cpu_nums = std::thread::hardware_concurrency();
    ThreadPool pool(4, ThreadPriority::HIGH, cpu_nums - 1);
    std::vector<std::future<int32_t>> results;

    for (int32_t i = 0; i < 8; ++i)
    {
        results.emplace_back(pool.Enqueue([i] {
#ifdef __linux__
            std::cout << "ThreadPoolTests::PriorityAndCpuTest thread=" << i
                      << " running on CPU=" << sched_getcpu() << std::endl;
#endif
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i * i;
        }));
    }

    int32_t ret = 0;
    for (auto&& result : results)
    {
        EXPECT_EQ(result.get(), ret * ret);
        ret++;
    }
}

TEST(ThreadPoolTests, ThreadCountTest)
{
    ThreadPool pool(4, ThreadPriority::NORMAL);
    EXPECT_EQ(pool.ThreadCount(), 4u);
}

TEST(ThreadPoolTests, QueueSizeTest)
{
    ThreadPool pool(1, ThreadPriority::NORMAL);

    std::mutex mtx;
    std::condition_variable cv;
    bool started = false;
    bool release = false;

    // Occupy the single worker thread so subsequently enqueued tasks pile up in the queue.
    auto blocker = pool.Enqueue([&] {
        std::unique_lock<std::mutex> lock(mtx);
        started = true;
        cv.notify_one();
        cv.wait(lock, [&] { return release; });
    });

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return started; });
    }

    std::vector<std::future<int32_t>> results;
    for (int32_t i = 0; i < 3; ++i)
        results.emplace_back(pool.Enqueue([i] { return i; }));

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    while (pool.QueueSize() != 3u && std::chrono::steady_clock::now() < deadline)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_EQ(pool.QueueSize(), 3u);

    {
        std::unique_lock<std::mutex> lock(mtx);
        release = true;
    }
    cv.notify_one();
    blocker.get();

    for (auto&& result : results)
        result.get();
}

#ifdef __linux__
TEST(ThreadPoolTests, SetGetThreadPriorityTest)
{
    // SCHED_RR requires CAP_SYS_NICE / root; skip gracefully when not permitted.
    int32_t rv = SetThreadPriority(ThreadPriority::HIGH);
    if (rv != 0)
        GTEST_SKIP() << "Insufficient privileges to set real-time thread priority";

    int32_t priority = GetThreadPriority(pthread_self());
    EXPECT_EQ(priority, static_cast<int32_t>(ThreadPriority::HIGH));
}
#endif
