/**************************************************************************
 * @file: PropertyPathTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#include <common/PropertyPath.h>
#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>

using namespace cppbase;

struct SimpleType
{
    int value;
    std::string name;
    std::vector<SimpleType> children;
    std::unordered_map<std::string, SimpleType*> children_map;

    SimpleType() = default;
    SimpleType(int v, const std::string& n) : value(v), name(n) {}
    SimpleType& operator=(const SimpleType& other)
    {
        value = other.value;
        name = other.name;
        children = other.children;
        children_map = other.children_map;
        return *this;
    }
};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<SimpleType>("SimpleType")
        .property("value", &SimpleType::value)
        .property("name", &SimpleType::name)
        .property("children", &SimpleType::children)
        .property("children_map", &SimpleType::children_map);
}

TEST(PropertyPathTests, PropertyInfo)
{
    PropertyInfo a;
    EXPECT_FALSE(a.IsValid());
    EXPECT_FALSE(a.GetType().is_valid());

    PropertyInfo b(Variant::GetType<SimpleType>(), "value");
    EXPECT_TRUE(b.IsValid());
    EXPECT_TRUE(b.GetType().is_valid());
    EXPECT_EQ(b.GetType(), Variant::GetType<int>());
    EXPECT_EQ(b.property.get_name(), "value");
    EXPECT_EQ(b.name, "value");
}

TEST(PropertyPathTests, Constructors)
{
    PropertyPath path_empty(Variant::GetType<SimpleType>());
    PropertyPath path_value{Variant::GetType<SimpleType>(), "value"};
    PropertyPath path_name{Variant::GetType<SimpleType>(), "name"};
    PropertyPath path_children{Variant::GetType<SimpleType>(), "children"};
    PropertyPath path_child_0{Variant::GetType<SimpleType>(), "children[0]"};
    PropertyPath path_child_a{Variant::GetType<SimpleType>(), "children_map[a]"};
    PropertyPath path_child_a_value{Variant::GetType<SimpleType>(), "children_map[a].value"};

    EXPECT_THROW(PropertyPath(Variant::GetType<SimpleType>(), "invalid"), std::runtime_error);
    EXPECT_THROW(PropertyPath(Variant::GetType<SimpleType>(), "value.invalid"), std::runtime_error);

    EXPECT_EQ(path_value.ToString(), "value");
    EXPECT_EQ(path_name.ToString(), "name");
    EXPECT_EQ(path_children.ToString(), "children");
    EXPECT_EQ(path_child_0.ToString(), "children[0]");
    EXPECT_EQ(path_child_a.ToString(), "children_map[a]");
    EXPECT_EQ(path_child_a_value.ToString(), "children_map[a].value");
    EXPECT_TRUE(path_empty.IsEmpty());
    EXPECT_FALSE(path_value.IsEmpty());

    EXPECT_EQ(path_empty.GetType(), Variant::GetType<SimpleType>());
    EXPECT_EQ(path_child_a_value.GetType(), Variant::GetType<int>());

    EXPECT_EQ(path_empty.GetRootType(), Variant::GetType<SimpleType>());
    EXPECT_EQ(path_child_a_value.GetRootType(), Variant::GetType<SimpleType>());
}

TEST(PropertyPathTests, GetSetValue)
{
    SimpleType obj;
    obj.name = "obj";
    obj.value = 1;

    obj.children.emplace_back(2, "child_0");
    obj.children_map.emplace("a", new SimpleType({3, "child_a"}));

    PropertyPath path_value{Variant::GetType<SimpleType>(), "value"};
    PropertyPath path_name{Variant::GetType<SimpleType>(), "name"};
    PropertyPath path_children{Variant::GetType<SimpleType>(), "children"};
    PropertyPath path_child_0{Variant::GetType<SimpleType>(), "children[0]"};
    PropertyPath path_children_map{Variant::GetType<SimpleType>(), "children_map"};
    PropertyPath path_child_a{Variant::GetType<SimpleType>(), "children_map[a]"};
    PropertyPath path_child_a_value{Variant::GetType<SimpleType>(), "children_map[a].value"};

    // Get value
    auto value = path_value.GetValue(obj);
    ASSERT_TRUE(value.is_valid());
    EXPECT_EQ(value.get_value<int>(), obj.value);
    auto name = path_name.GetValue(obj);
    ASSERT_TRUE(name.is_valid());
    EXPECT_EQ(name.get_value<std::string>(), obj.name);

    auto children = path_children.GetValue(obj);
    ASSERT_TRUE(children.is_valid());
    auto children_vec = children.get_value<std::vector<SimpleType>>();
    EXPECT_EQ(children_vec.size(), 1);

    auto children_map = path_children_map.GetValue(obj);
    ASSERT_TRUE(children_map.is_valid());
    auto children_map_val = children_map.get_value<std::unordered_map<std::string, SimpleType*>>();
    EXPECT_EQ(children_map_val.size(), 1);

    auto child_0 = path_child_0.GetValue(obj);
    ASSERT_TRUE(child_0.is_valid());
    EXPECT_EQ(child_0.get_type(), Variant::GetType<SimpleType>());
    EXPECT_EQ(child_0.get_value<SimpleType>().value, 2);
    EXPECT_EQ(child_0.get_value<SimpleType>().name, "child_0");

    auto child_a = path_child_a.GetValue(obj);
    ASSERT_TRUE(child_a.is_valid());
    EXPECT_EQ(child_a.get_type(), Variant::GetType<SimpleType*>());
    EXPECT_EQ(child_a.get_value<SimpleType*>()->value, 3);
    EXPECT_EQ(child_a.get_value<SimpleType*>()->name, "child_a");

    // Set value
    EXPECT_TRUE(path_value.SetValue(obj, 4));
    EXPECT_EQ(obj.value, 4);
    EXPECT_TRUE(path_name.SetValue(obj, std::string("obj_new")));
    EXPECT_EQ(obj.name, "obj_new");
    SimpleType child_0_new({5, "child_0_new"});
    EXPECT_TRUE(path_child_0.SetValue(obj, child_0_new));
    EXPECT_EQ(obj.children[0].value, 5);
    EXPECT_EQ(obj.children[0].name, "child_0_new");
    SimpleType child_a_new({6, "child_a_new"});
    EXPECT_TRUE(path_child_a.SetValue(obj, &child_a_new));
    EXPECT_EQ(obj.children_map["a"]->value, 6);
    EXPECT_EQ(obj.children_map["a"]->name, "child_a_new");
    EXPECT_TRUE(path_child_a_value.SetValue(obj, 7));
    EXPECT_EQ(obj.children_map["a"]->value, 7);
}
