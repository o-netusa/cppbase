/**************************************************************************
 * @file: ForkJoinPoolTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 * All rights reserved.
 *************************************************************************/

#include <common/ForkJoinPool.h>
#include <gtest/gtest.h>

#include <iostream>

static void foo(const uint64_t begin, uint64_t *result)
{
    uint64_t prev[] = {begin, 0};
    for (uint64_t i = 0; i < 1000000000; ++i)
    {
        const auto tmp = (prev[0] + prev[1]) % 1000;
        prev[1] = prev[0];
        prev[0] = tmp;
    }
    *result = prev[0];
}

void batch(asio::fork_join_pool &pool, const uint64_t (&a)[2])
{
    uint64_t r[] = {0, 0};
    {
        asio::fork_executor fork(pool);
        asio::join_guard join(fork);
        asio::execution::execute(fork, [&a, &r]() { foo(a[0], &r[0]); });
        asio::execution::execute(fork, [&a, &r]() { foo(a[1], &r[1]); });
        // fork.join(); // or let join_guard destructor run
    }
    std::cerr << "foo(" << a[0] << "): " << r[0] << " foo(" << a[1] << "): " << r[1] << std::endl;
}

TEST(ForkJoinPoolTests, ThreadPoolTest)
{
    asio::fork_join_pool pool(4);

    batch(pool, {2, 4});
    batch(pool, {3, 5});
    batch(pool, {7, 9});
}
