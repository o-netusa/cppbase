/**************************************************************************
 * @file: PropertyPath.impl
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <assert.h>
#include <common/Encoding.h>
#include <rttr/variant_associative_view.h>
#include <rttr/variant_sequential_view.h>

namespace cppbase { namespace internal {

struct PropertyPathImpl
{
    Variant::Type m_type;
    std::vector<PropertyInfo> m_path;

    PropertyPathImpl(Variant::Type type, const std::initializer_list<PropertyInfo>& list)
        : m_type(type), m_path(list)
    {}

    PropertyPathImpl(Variant::Type type, const std::string& path) : m_type(type)
    {
        if (!path.empty())
        {
            auto tokens = SplitString(path, '.');
            for (auto& token : tokens)
            {
                if (token.empty())
                {
                    continue;
                }
                if (token.back() == ']')
                {
                    auto bracket_pos = token.find('[');
                    if (bracket_pos == std::string::npos)
                    {
                        throw std::runtime_error("Invalid path: " + path);
                    }
                    auto prop_name = token.substr(0, bracket_pos);
                    auto property = type.get_property(prop_name);
                    if (!property.is_valid())
                    {
                        throw std::runtime_error("Invalid path: " + path);
                    }
                    if (property.get_type().is_sequential_container() &&
                        property.get_type().is_template_instantiation())
                    {
                        auto idx = std::stoi(
                            token.substr(bracket_pos + 1, token.size() - bracket_pos - 2));
                        m_path.emplace_back(type, prop_name, idx);
                        type = *(property.get_type().get_template_arguments().begin());
                    } else if (property.get_type().is_associative_container() &&
                               property.get_type().is_template_instantiation())
                    {
                        auto key = token.substr(bracket_pos + 1, token.size() - bracket_pos - 2);
                        m_path.emplace_back(type, prop_name, key);
                        auto t = property.get_type().get_template_arguments().begin();
                        type = *(++t);
                    } else
                    {
                        throw std::runtime_error("Invalid path: " + path);
                    }
                } else
                {
                    m_path.emplace_back(type, token);
                    type = type.get_property(token).get_type();
                }
            }
        }
    }

    bool operator==(const PropertyPathImpl& rhs) const { return m_path == rhs.m_path; }

    bool IsValid() const
    {
        if (m_type.is_valid())
        {
            for (const auto& info : m_path)
            {
                if (!info.IsValid())
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    Variant::Value GetInstanceValue(Variant::Instance obj, const PropertyInfo& prop_info)
    {
        if (!prop_info.IsValid())
        {
            return Variant::Value();
        }

        auto value = prop_info.property.get_value(obj);
        if (prop_info.property.get_type().is_sequential_container())
        {
            rttr::variant_sequential_view view = value.create_sequential_view();
            assert(prop_info.index >= 0 && prop_info.index < view.get_size());
            value = view.get_value(prop_info.index).extract_wrapped_value();
        } else if (prop_info.property.get_type().is_associative_container())
        {
            rttr::variant_associative_view view = value.create_associative_view();
            assert(!prop_info.key.empty());
            for (const auto& item : view)
            {
                if (item.first.extract_wrapped_value().to_string() == prop_info.key)
                {
                    value = item.second.extract_wrapped_value();
                    break;
                }
            }
        }
        return value;
    }

    Variant::Value GetValue(Variant::Instance obj)
    {
        if (!IsValid() || !obj.is_valid())
        {
            return Variant::Value();
        }

        Variant::Value value;
        for (uint32_t idx = 0; idx < m_path.size(); ++idx)
        {
            auto& info = m_path[idx];
            assert(info.IsValid());
            value = GetInstanceValue((idx == 0) ? obj : Variant::Instance(value), info);
        }
        return value;
    }

    bool SetValue(Variant::Instance obj, Variant::Argument arg, uint32_t prop_idx)
    {
        if (!IsValid() || !obj.is_valid() || prop_idx >= m_path.size())
        {
            return false;
        }

        auto& info = m_path[prop_idx];
        auto value = info.property.get_value(obj);
        assert(value.is_valid());
        bool ret = false;
        if (info.property.get_type().is_associative_container())
        {
            assert(!info.key.empty());
            rttr::variant_associative_view view = value.create_associative_view();
            auto iter = view.find(info.key);
            if (iter == view.end())
            {
                return ret;
            }
            if (prop_idx == m_path.size() - 1)
            {
                view.erase(info.key);
                ret = view.insert(info.key, arg).second;
            } else
            {
                ret = SetValue(iter.get_value().extract_wrapped_value(), arg, prop_idx + 1);
            }
            if (ret)
            {
                return info.property.set_value(obj, value);
            }
        } else if (info.property.get_type().is_sequential_container())
        {
            rttr::variant_sequential_view view = value.create_sequential_view();
            assert(info.index >= 0 && info.index < view.get_size());
            if (prop_idx == m_path.size() - 1)
            {
                ret = view.set_value(info.index, arg);
            } else
            {
                ret =
                    SetValue(view.get_value(info.index).extract_wrapped_value(), arg, prop_idx + 1);
            }
            if (ret)
            {
                return info.property.set_value(obj, value);
            }
        } else
        {
            if (prop_idx == m_path.size() - 1)
            {
                ret = info.property.set_value(obj, arg);
            } else
            {
                ret = SetValue(value, arg, prop_idx + 1);
                if (ret)
                {
                    ret = info.property.set_value(obj, value);
                }
            }
        }
        return ret;
    }

    bool SetValue(Variant::Instance obj, Variant::Argument arg)
    {
        if (!IsValid() || !obj.is_valid())
        {
            return false;
        }

        return SetValue(obj, arg, 0);
    }

    std::string ToString() const
    {
        std::string str;
        for (uint32_t idx = 0; idx < m_path.size(); ++idx)
        {
            auto& info = m_path[idx];
            str += info.name;
            if (info.index >= 0)
            {
                str += "[" + std::to_string(info.index) + "]";
            } else if (!info.key.empty())
            {
                str += "[" + info.key + "]";
            }
            if (idx < m_path.size() - 1)
            {
                str += ".";
            }
        }
        return str;
    }

    Variant::Type GetType() const
    {
        if (m_path.empty())
        {
            return m_type;
        }
        return m_path.back().GetType();
    }
};

}}  // namespace cppbase::internal
