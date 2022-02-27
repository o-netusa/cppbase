/**************************************************************************
 * @file: Variant_impl.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 * All rights reserved.
 *************************************************************************/

#include <rttr/variant.h>
#include <rttr/type>

namespace internal {

struct Impl
{
    using Value = rttr::variant;
    using Type = rttr::type;

    Impl() = default;
    ~Impl() = default;

    template <typename T, typename = std::enable_if_t<!std::is_reference_v<T> &&
                                                      !std::is_same_v<rttr::variant, T>>>
    Impl(T&& val) : m_val(std::move(val))
    {}

    template <typename T, typename = std::enable_if_t<!std::is_same_v<rttr::variant, T>>>
    Impl(T& val) : m_val(val)
    {}

    Impl(Impl&& other) : m_val(std::move(other.m_val)) {}

    Impl(const Impl& other) : m_val(other.m_val) {}

    Impl& operator=(Impl&& other)
    {
        m_val = std::move(other.m_val);
        return *this;
    }

    Impl& operator=(const Impl& other)
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

    bool operator==(const Impl& other) const { return m_val == other.m_val; }

    bool operator!=(const Impl& other) const { return m_val != other.m_val; }

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

}  // namespace internal
