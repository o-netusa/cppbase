/**************************************************************************
 * @file: PropertyPath.h
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <string>

#include "Variant.h"

namespace cppbase {

/**
 * @brief PropertyInfo defines the information of a property of a type.
 * e.g.
 *   struct SimpleType
 *   {
 *       int value;
 *       std::string name;
 *       std::vector<SimpleType> children;
 *       std::unordered_map<std::string, SimpleType*> children_map;
 *   };
 *   PropertyInfo b(Variant::GetType<SimpleType>(), "value");
 *
 *   b represents the property "value" of type SimpleType.
 */
struct PropertyInfo
{
    PropertyInfo() = default;

    PropertyInfo(Variant::Type type, const std::string &prop_name, int idx = -1)
    {
        auto properties = type.get_properties();
        for (auto prop : properties)
        {
            if (prop.get_name().to_string() == prop_name)
            {
                property = prop;
                name = prop_name;
                if (property.get_type().is_sequential_container())
                {
                    index = idx;
                }
                break;
            }
        }
    }

    PropertyInfo(Variant::Type type, const std::string &prop_name, const std::string &k)
    {
        auto properties = type.get_properties();
        for (auto prop : properties)
        {
            if (prop.get_name().to_string() == prop_name &&
                prop.get_type().is_associative_container())
            {
                property = prop;
                name = prop_name;
                key = k;
                break;
            }
        }
    }

    PropertyInfo(const Variant::Property &prop) : name(prop.get_name().to_string()), property(prop)
    {}

    ~PropertyInfo() = default;

    bool operator==(const PropertyInfo &rhs) const
    {
        return (name == rhs.name) && (property == rhs.property);
    }

    bool IsValid() const { return (!name.empty()) && property.is_valid(); }

    Variant::Type GetType() const { return property.get_type(); }

    Variant::Property property{VARIANT_NULL_PROPERTY};
    std::string name;
    // sequetial container index
    int index{-1};
    // associative container key
    std::string key;
};

}  // namespace cppbase

#ifdef USE_RTTR
#include "rttr/PropertyPath.impl.h"
#else
#include "std/PropertyPath.impl.h"
#endif

namespace cppbase {

/**
 * @brief PropertyPath defines a chained PropertyInfos that can be used to access a property of a type.
 * e.g.
 *   struct SimpleType
 *   {
 *       int value;
 *       std::string name;
 *       std::vector<SimpleType> children;
 *       std::unordered_map<std::string, SimpleType*> children_map;
 *   };
 *   PropertyPath path_child_a_value{Variant::GetType<SimpleType>(), "children_map[a].value"};
 *
 *   path_child_a_value can be used to access "SimpleType.children_map[a].value".
 *   The client code can use GetValue() and SetValue() to access the property of a SimpleType instance.
 */
class PropertyPath
{
public:
    PropertyPath() = default;
    ~PropertyPath() = default;

    PropertyPath(Variant::Type type, const std::initializer_list<PropertyInfo> &list)
        : m_impl(type, list)
    {}
    PropertyPath(Variant::Type type, const std::string &path = "") : m_impl(type, path) {}

    PropertyPath(const PropertyPath &rhs) : m_impl(rhs.m_impl) {}

    PropertyPath &operator=(const PropertyPath &rhs)
    {
        m_impl = rhs.m_impl;
        return *this;
    }

    PropertyPath(PropertyPath &&rhs) noexcept : m_impl(std::move(rhs.m_impl)) {}

    PropertyPath &operator=(PropertyPath &&rhs) noexcept
    {
        m_impl = std::move(rhs.m_impl);
        return *this;
    }

    bool operator==(const PropertyPath &rhs) const { return m_impl == rhs.m_impl; }

    bool IsValid() const { return m_impl.IsValid(); }

    /**
     * @brief 获取obj中访问路径对应属性值
     */
    Variant::Value GetValue(const Variant &obj) { return m_impl.GetValue(obj); }
    /**
     * @brief 设置obj中访问路径对应属性值
     */
    bool SetValue(Variant::Instance obj, Variant::Argument arg)
    {
        return m_impl.SetValue(obj, arg);
    }

    std::string ToString() const { return m_impl.ToString(); }

    Variant::Type GetType() const { return m_impl.GetType(); }

    Variant::Type GetRootType() const { return m_impl.GetRootType(); }

    bool IsEmpty() const { return m_impl.m_path.empty(); }

private:
    internal::PropertyPathImpl m_impl;
};

}  // namespace cppbase
