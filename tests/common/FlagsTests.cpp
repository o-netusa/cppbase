/**************************************************************************
 * @file:  FlagsTests.cpp
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
**************************************************************************/

#include <gtest/gtest.h>
#include <common/Flags.h>

using namespace cppbase;

enum class FooFlag : int
{
    FLAG_A = 0x1,
    FLAG_B = 0x2,
    FLAG_C = 0x4,
    FLAG_D = 0x8,
    FLAG_E = 0x10,
    FLAG_F = 0x20
};
DECLARE_FLAGS(FooFlags, FooFlag);

TEST(FlagsTests, FlagsTests)
{
    // build flags
    FooFlags flags{FooFlag::FLAG_A, FooFlag::FLAG_B, FooFlag::FLAG_C};
    EXPECT_EQ(static_cast<int>(flags), 0x1 | 0x2 | 0x4);

    // check flags
    EXPECT_TRUE(flags & FooFlag::FLAG_A);
    EXPECT_FALSE(flags & FooFlag::FLAG_D);
    EXPECT_TRUE(flags.CheckFlag(FooFlag::FLAG_B));
    EXPECT_FALSE(flags.CheckFlag(FooFlag::FLAG_E));

    EXPECT_TRUE(flags.CheckFlags({FooFlag::FLAG_C, FooFlag::FLAG_E, FooFlag::FLAG_F}));
    EXPECT_FALSE(flags.CheckFlags({FooFlag::FLAG_D, FooFlag::FLAG_E, FooFlag::FLAG_F}));

    // set flags
    flags |= FooFlag::FLAG_D; // set D on
    EXPECT_TRUE(flags & FooFlag::FLAG_D);
    flags.SetFlag(FooFlag::FLAG_D, false); // set D off
    EXPECT_FALSE(flags & FooFlag::FLAG_D);
    flags.SetFlags({FooFlag::FLAG_A, FooFlag::FLAG_B}, false); // set A/B off
    EXPECT_EQ(static_cast<int>(flags), 0x4);
    flags |= {FooFlag::FLAG_E, FooFlag::FLAG_F}; // set E/F on
    EXPECT_EQ(static_cast<int>(flags), 0x4 | 0x10 | 0x20);
    flags &= ~FooFlags{FooFlag::FLAG_E, FooFlag::FLAG_F}; // set E/F off
    EXPECT_EQ(static_cast<int>(flags), 0x4);
}
