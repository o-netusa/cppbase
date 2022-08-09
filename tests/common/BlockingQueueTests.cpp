/**************************************************************************
 * @file:  BlockingQueueTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 **************************************************************************/

#include <common/BlockingQueue.h>
#include <gtest/gtest.h>

using namespace cppbase;

TEST(BlockingQueueTests, MultiThreadTest)
{
    BlockingQueue<int32_t> queue;
}
