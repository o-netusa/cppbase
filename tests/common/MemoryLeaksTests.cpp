/**************************************************************************
 * @file:  EncodingTests.cpp
 * @brief:
 *
 * Copyright 2021 O-Net Technologies (Group) Limited.
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
