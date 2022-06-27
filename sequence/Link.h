/**************************************************************************
 * @file: Link.h
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <common/Uuid.h>

namespace cppbase { namespace sequence {

struct Link
{
    explicit Link(uuids::uuid src, uuids::uuid dst, uint32_t src_id = 0, uint32_t dst_id = 0)
      : m_src(src), m_dst(dst), m_src_id(src_id), m_dst_id(dst_id)
    {}
    Link() = default;
    ~Link() = default;

    Link(const Link& other)
    {
        m_src = other.m_src;
        m_dst = other.m_dst;
        m_src_id = other.m_src_id;
        m_dst_id = other.m_dst_id;
    }

    Link& operator=(const Link& other)
    {
        m_src = other.m_src;
        m_dst = other.m_dst;
        m_src_id = other.m_src_id;
        m_dst_id = other.m_dst_id;
        return *this;
    }

    bool operator==(const Link& other)
    {
        return (m_src == other.m_src && m_dst == other.m_dst &&
                m_src_id == other.m_src_id && m_dst_id == other.m_dst_id);
    }

    bool operator!=(const Link& other)
    {
        return !(*this == other);
    }

    uuids::uuid m_src;
    uuids::uuid m_dst;
    uint32_t m_src_id{0};
    uint32_t m_dst_id{0};

    template<class Archive>
    void serialize(Archive& archive, const uint32_t)
    {
        archive(m_src, m_dst, m_src_id, m_dst_id);
    }
};

}} // namespaces
