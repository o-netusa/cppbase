/**************************************************************************
 * @file:  UuidTests.cpp
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 **************************************************************************/

#include <common/Uuid.h>
#include <gtest/gtest.h>

using namespace uuids;

TEST(UuidTests, Basic)
{
    uuid empty;
    EXPECT_TRUE(empty.is_nil());
    EXPECT_EQ("00000000-0000-0000-0000-000000000000", to_string(empty));
}

TEST(UuidTests, Parse)
{
    {
        auto guid = uuids::uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value();
        EXPECT_EQ("47183823-2574-4bfd-b411-99ed177d3e43", uuids::to_string(guid));
    }
    {
        auto str = "";
        EXPECT_FALSE(uuids::uuid::is_valid_uuid(str));
    }
    {
        auto guid = uuids::uuid::from_string(L"47183823-2574-4bfd-b411-99ed177d3e43").value();
        EXPECT_EQ("47183823-2574-4bfd-b411-99ed177d3e43", uuids::to_string(guid));
    }
    {
        auto guid = uuids::uuid::from_string("00000000-0000-0000-0000-000000000000").value();
        EXPECT_EQ("00000000-0000-0000-0000-000000000000", uuids::to_string(guid));
    }
}

TEST(UuidTests, Generator)
{
    uuid empty;
    auto guid1 = cppbase::Uuid::Generate();
    EXPECT_NE(guid1, empty);
    auto guid2 = cppbase::Uuid::Generate();
    EXPECT_NE(guid2, guid1);
}
