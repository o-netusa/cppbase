/**************************************************************************
 * @file:  BlockingQueueTests.cpp
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
**************************************************************************/

#include <common/BlockingQueue.h>
#include <gtest/gtest.h>

using namespace cppbase;

TEST(BlockingQueueTests, MultiThreadTest)
{
    BlockingQueue<int32_t> queue;
}
