/**************************************************************************
 * @file:  ThreadPoolTests.cpp
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 **************************************************************************/

#include <common/ThreadPool.h>
#include <gtest/gtest.h>

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
