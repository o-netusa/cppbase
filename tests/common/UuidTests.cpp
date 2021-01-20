/**************************************************************************
 * @file:  UuidTests.cpp
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
**************************************************************************/

#include <gtest/gtest.h>
#include <uuid.h>

using namespace uuids;

TEST(UuidTests, Basic)
{
    uuid empty;
    EXPECT_TRUE(empty.is_nil());
    EXPECT_EQ("00000000-0000-0000-0000-000000000000", to_string(empty));
}

TEST(UuidTests, Parse)
{
   EXPECT_TRUE(uuid::is_valid_uuid("47183823-2574-4bfd-b411-99ed177d3e43"));
   EXPECT_TRUE(uuid::is_valid_uuid("{47183823-2574-4bfd-b411-99ed177d3e43}"));
   EXPECT_TRUE(uuid::is_valid_uuid(L"47183823-2574-4bfd-b411-99ed177d3e43"));
   EXPECT_TRUE(uuid::is_valid_uuid(L"{47183823-2574-4bfd-b411-99ed177d3e43}"));
   EXPECT_TRUE(uuid::is_valid_uuid("00000000-0000-0000-0000-000000000000"));
   EXPECT_TRUE(uuid::is_valid_uuid("{00000000-0000-0000-0000-000000000000}"));
   EXPECT_TRUE(uuid::is_valid_uuid(L"00000000-0000-0000-0000-000000000000"));
   EXPECT_TRUE(uuid::is_valid_uuid(L"{00000000-0000-0000-0000-000000000000}"));
}
