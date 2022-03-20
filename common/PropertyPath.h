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

class PropertyPath
{
public:
    PropertyPath() = default;
    PropertyPath(Variant::Type type, const std::initializer_list<PropertyInfo> &list)
        : m_impl(type, list)
    {}
    PropertyPath(Variant::Type type, const std::string &path = "") : m_impl(type, path) {}
    ~PropertyPath() = default;

    bool operator==(const PropertyPath &rhs) const { return m_impl == rhs.m_impl; }

    bool IsValid() const { return m_impl.IsValid(); }

    /**
     * @brief 获取obj中访问路径对应属性值
     */
    Variant::Value GetValue(Variant::Instance obj) { return m_impl.GetValue(obj); }
    /**
     * @brief 设置obj中访问路径对应属性值
     */
    bool SetValue(Variant::Instance obj, Variant::Argument arg)
    {
        return m_impl.SetValue(obj, arg);
    }

    std::string ToString() const { return m_impl.ToString(); }

    Variant::Type GetType() const { return m_impl.GetType(); }

    bool IsEmpty() const { return m_impl.m_path.empty(); }

private:
    internal::PropertyPathImpl m_impl;
};

}  // namespace cppbase
