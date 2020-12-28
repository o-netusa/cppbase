/*****************************************************************************
 * @file: Flags.h
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
 ****************************************************************************/

#pragma once

#include <type_traits>
#include <initializer_list>

namespace cppbase {

template<typename FLAG_TYPE>
class Flags
{
    static_assert(std::is_enum_v<FLAG_TYPE>,
                  "onet_base::common::Flags is only applicable on enumeration types.");
    static_assert((sizeof(FLAG_TYPE) == sizeof(int)),
                  "onet_base::common::Flags uses int as storage, "
                  "consider using enum class FLAG_TYPE : int.");

public:
    Flags() = default;
    Flags(FLAG_TYPE flag) : m_flags(static_cast<int>(flag)) {}
    Flags(std::initializer_list<FLAG_TYPE> flags)
    {
        for (auto flag : flags)
        {
            m_flags |= static_cast<int>(flag);
        }
    }
    ~Flags() = default;

    inline Flags &operator=(Flags other) { m_flags = other.m_flags; return *this; }

    inline Flags &operator&=(Flags mask) { m_flags &= mask.m_flags; return *this; }
    inline Flags &operator|=(Flags other) { m_flags |= other.m_flags; return *this; }
    inline Flags &operator^=(Flags other) { m_flags ^= other.m_flags; return *this; }

    inline Flags &operator&=(FLAG_TYPE mask) { m_flags &= static_cast<int>(mask); return *this; }
    inline Flags &operator|=(FLAG_TYPE other) { m_flags |= static_cast<int>(other); return *this; }
    inline Flags &operator^=(FLAG_TYPE other) { m_flags ^= static_cast<int>(other); return *this; }

    inline Flags operator&(Flags mask) const { return Flags{m_flags & mask.m_flags}; }
    inline Flags operator|(Flags other) const { return Flags{m_flags | other.m_flags}; }
    inline Flags operator^(Flags other) const { return Flags{m_flags ^ other.m_flags}; }

    inline Flags operator&(FLAG_TYPE mask) const { return Flags{m_flags & static_cast<int>(mask)}; }
    inline Flags operator|(FLAG_TYPE other) const { return Flags{m_flags | static_cast<int>(other)}; }
    inline Flags operator^(FLAG_TYPE other) const { return Flags{m_flags ^ static_cast<int>(other)}; }

    inline Flags operator~() const { return Flags{~m_flags}; }
    inline operator int() const { return m_flags; }
    inline operator bool() const { return (m_flags != 0); }
    inline bool operator!() const { return m_flags == 0; }

    Flags &SetFlags(Flags flags, bool on = true)
    {
        return on ? (*this |= flags) : (*this &= ~flags);
    }
    Flags &SetFlag(FLAG_TYPE flag, bool on = true)
    {
        return on ? (*this |= flag) : (*this &= ~static_cast<int>(flag));
    }
    bool CheckFlags(Flags flags) const
    {
        return *this & flags;
    }
    bool CheckFlag(FLAG_TYPE flag) const
    {
        return *this & flag;
    }

private:
    inline Flags(int flags) : m_flags(flags) {}
    inline Flags &operator&=(int mask) { m_flags &= mask; return *this; }
    int m_flags{0};
};

} // namespace cppbase

#define DECLARE_FLAGS(XFlags, XFlag) typedef cppbase::Flags<XFlag> XFlags
