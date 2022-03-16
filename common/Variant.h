/**************************************************************************
 * @file: Variant.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <memory>
#include <type_traits>

#include "Global.h"

#ifdef USE_RTTR
#include "rttr/Variant.impl.h"
#define ENABLE_TYPE_INFO RTTR_ENABLE
#else
#include "std/Variant.impl.h"
#endif

namespace cppbase {

/**
 * @brief The Variant class allows to store data of any type and convert between these types
 *   transparently.
 * @remark The content is copied into the variant class.
 * @note When build with USE_RTTR option, uses rttr internally, otherwise use std::any.
 */
class Variant
{
public:
    using Type = internal::Type;
    using Value = internal::Value;
    using Instance = internal::Instance;
    using Argument = internal::Argument;
    using Property = internal::Property;

    template <typename T>
    static Type GetType()
    {
        return internal::VariantImpl::GetType<T>();
    }

    Variant() = default;
    ~Variant() = default;

    /**
     * @brief Construct a Variant object with the given value.
     * @note The value will be copied or moved into the variant.
     */
    template <typename T,
              typename = std::enable_if_t<!std::is_reference_v<T> && !std::is_same_v<Variant, T>>>
    Variant(T&& val) : m_impl(std::move(val))
    {}

    template <typename T, typename = std::enable_if_t<!std::is_same_v<Variant, T>>>
    Variant(T& val) : m_impl(val)
    {}

    Variant(const Variant& other) : m_impl(other.m_impl) {}

    Variant(Variant&& other) : m_impl(std::move(other.m_impl)) {}

    template <typename T, typename = std::enable_if_t<!std::is_same_v<T, Variant>>>
    Variant& operator=(T& val)
    {
        m_impl = val;
        return *this;
    }

    Variant& operator=(Variant&& other)
    {
        m_impl = std::move(other.m_impl);
        return *this;
    }

    Variant& operator=(const Variant& other)
    {
        m_impl = other.m_impl;
        return *this;
    }

    bool operator==(const Variant& other) const { return m_impl == other.m_impl; }

    bool operator!=(const Variant& other) const { return m_impl != other.m_impl; }

    explicit operator bool() const { return m_impl.IsValid(); }

    void Clear() { m_impl.Clear(); }

    template <typename T>
    bool IsType() const
    {
        return m_impl.IsType<T>();
    }

    bool IsValid() const { return m_impl.IsValid(); }

    template <typename T>
    const T& GetValue() const
    {
        return m_impl.GetValue<T>();
    }

    template <typename T>
    T& GetValue()
    {
        return m_impl.GetValue<T>();
    }

    Value GetInternalValue() { return m_impl.m_val; }

    Type GetType() const { return m_impl.GetType(); }

private:
    internal::VariantImpl m_impl;
};

}  // namespace cppbase
