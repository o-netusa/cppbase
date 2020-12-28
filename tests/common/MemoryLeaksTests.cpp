/**************************************************************************
 * @file:  EncodingTests.cpp
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
**************************************************************************/

#include <common/MemoryLeaks.h>
#include <gtest/gtest.h>

TEST(MemoryLeaksTests, NewLeakTest)
{
    [[maybe_unused]] int* ptr = nullptr;

    // Make some memory leaks
    ptr = new int;
    ptr = new int[100];
}
