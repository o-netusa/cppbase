/**************************************************************************
 * @file: VariantTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 * All rights reserved.
 *************************************************************************/

#include <gtest/gtest.h>
#include <common/Variant.h>

using namespace cppbase;

struct simple_type
{
    simple_type() {}
    simple_type(const simple_type& other)
    :   moved(other.moved),
        moved_from(other.moved_from)
    {
    }
    simple_type(simple_type&& other)
    :   moved(true)
    {
        other.moved_from = true;
    }

    bool moved = false;
    bool moved_from = false;
};

struct big_type : simple_type
{
    big_type() {}
    big_type(const big_type& other) : simple_type(other) {}

    big_type(big_type&& other) : simple_type(std::forward<simple_type>(other)) {}

    double value;
    double value_2;
};

TEST(Variant, Constructors)
{
    Variant a;
    EXPECT_FALSE(a.IsValid());
    EXPECT_FALSE((bool)a);
    EXPECT_FALSE(a.GetType().is_valid());

    Variant b = 42;
    EXPECT_TRUE(b.IsValid());
    EXPECT_TRUE((bool)b);
    EXPECT_TRUE(b.GetType().is_valid());
    EXPECT_TRUE(b.IsType<int>());

    Variant c = b;
    EXPECT_TRUE(c.IsValid());
    EXPECT_TRUE((bool)c);
    EXPECT_TRUE(c.GetType().is_valid());
    EXPECT_TRUE(c.IsType<int>());

    Variant d = std::move(b);
    EXPECT_FALSE(b.IsValid());
    EXPECT_TRUE(d.IsValid());
    EXPECT_TRUE((bool)d);
    EXPECT_TRUE(d.GetType().is_valid());
    EXPECT_TRUE(d.IsType<int>());
}

TEST(Variant, BasicAssignment)
{
    Variant a;
    Variant b;

    a = b;
    EXPECT_FALSE(a.IsValid());
    EXPECT_FALSE(a.GetType().is_valid());

    a = std::move(b);
    EXPECT_FALSE(a.IsValid());
    EXPECT_FALSE(a.GetType().is_valid());

    EXPECT_FALSE(b.IsValid());
    EXPECT_FALSE(b.GetType().is_valid());
}

TEST(Variant, SimpleType)
{
    simple_type obj;
    Variant var(obj);

    EXPECT_FALSE(obj.moved_from);

    Variant var_2(std::move(obj));
    EXPECT_TRUE(obj.moved_from);
    EXPECT_TRUE(var_2.GetValue<simple_type>().moved);
}

TEST(Variant, BigType)
{
    big_type obj;
    Variant var(obj);

    EXPECT_FALSE(obj.moved_from);

    Variant var_2(std::move(obj));
    EXPECT_TRUE(obj.moved_from);
    EXPECT_TRUE(var_2.GetValue<simple_type>().moved);
}
