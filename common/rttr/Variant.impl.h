/**************************************************************************
 * @file: Variant.impl.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 * All rights reserved.
 *************************************************************************/

#include <rttr/property.h>
#include <rttr/variant.h>

#include <rttr/registration>
#include <rttr/type>

#define VARIANT_NULL_PROPERTY rttr::type::get_by_name("").get_property("")

namespace cppbase { namespace internal {

using Value = rttr::variant;
using Type = rttr::type;
using Instance = rttr::instance;
using Argument = rttr::argument;
using Property = rttr::property;

struct VariantImpl
{
    VariantImpl() = default;
    ~VariantImpl() = default;

    template <typename T, typename = std::enable_if_t<!std::is_reference_v<T> &&
                                                      !std::is_same_v<rttr::variant, T>>>
    VariantImpl(T&& val) : m_val(std::move(val))
    {}

    template <typename T, typename = std::enable_if_t<!std::is_same_v<rttr::variant, T>>>
    VariantImpl(T& val) : m_val(val)
    {}

    VariantImpl(VariantImpl&& other) : m_val(std::move(other.m_val)) {}

    VariantImpl(const VariantImpl& other) : m_val(other.m_val) {}

    VariantImpl& operator=(VariantImpl&& other)
    {
        m_val = std::move(other.m_val);
        return *this;
    }

    VariantImpl& operator=(const VariantImpl& other)
    {
        m_val = other.m_val;
        return *this;
    }

    template <typename T>
    static Type GetType()
    {
        return rttr::type::get<T>();
    }

    template <typename T>
    const T& GetValue() const
    {
        return m_val.get_value<T>();
    }

    template <typename T>
    T& GetValue()
    {
        return m_val.get_value<T>();
    }

    bool operator==(const VariantImpl& other) const { return m_val == other.m_val; }

    bool operator!=(const VariantImpl& other) const { return m_val != other.m_val; }

    void Clear() { m_val.clear(); }

    template <typename T>
    bool IsType() const
    {
        return m_val.is_type<T>();
    }

    bool IsValid() const { return m_val.is_valid(); }

    rttr::type GetType() const { return m_val.get_type(); }

    rttr::variant m_val;
};

}}  // namespace cppbase::internal
