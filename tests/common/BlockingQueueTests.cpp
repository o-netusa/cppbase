/**************************************************************************
 * @file:  BlockingQueueTests.cpp
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
**************************************************************************/

#include <common/BlockingQueue.h>
#include <gtest/gtest.h>

using namespace cppbase;

TEST(BlockingQueueTests, MultiThreadTest)
{
    BlockingQueue<int32_t> queue;
}
