/**************************************************************************
 * @file:  EncodingTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
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
