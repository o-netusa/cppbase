/**************************************************************************
 * @file: Container.h
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <tuple>
#include <unordered_map>

#include "Processor.h"

namespace cppbase { namespace sequence {

class Container : public Processor
{
public:
    Container() = default;
    virtual ~Container() = default;

    virtual void AddProcessor(std::shared_ptr<Processor> processor)
    {
        if (!processor)
        {
            throw std::invalid_argument("Can't add null processor to Container");
        }

        m_child_param_idx[m_params.size()] = m_processors.size() - 1;
        m_params.push_back(processor->GetParam());

        processor->SetParent(this);
        m_processors.push_back(std::move(processor));
    }

    virtual void RemoveProcessor(const uuids::uuid& proc_id)
    {
        const auto& ret = std::find_if(m_processors.begin(), m_processors.end(),
            [proc_id](const auto& proc) { return (proc != nullptr && proc->GetId() == proc_id); });
        if (ret != m_processors.end())
        {
            (*ret)->SetParent(nullptr);
            m_processors.erase(ret);
        }
    }

    const Processor* GetProcessor(const uuids::uuid& proc_id) const
    {
        const auto& ret = std::find_if(m_processors.begin(), m_processors.end(),
            [proc_id](const auto& proc) { return proc && proc->GetId() == proc_id; });
        if (ret != m_processors.end())
        {
            return ret->get();
        }
        else
        {
            return nullptr;
        }
    }

    std::vector<Processor*> GetAllProcessors() const
    {
        std::vector<Processor*> vec;
        for (const auto& it : m_processors)
        {
            vec.push_back(it.get());
        }
        return vec;
    }

    template<typename T>
    std::vector<Processor*> GetProcessors() const
    {
        std::vector<Processor*> vec;
        for (const auto& it : m_processors)
        {
            if (dynamic_cast<T*>(it.get()))
            {
                vec.push_back(it.get());
            }
        }
        return vec;
    }

    virtual void Clear()
    {
        m_processors.clear();
        m_child_param_idx.clear();
    }

    void SetParam(const cppbase::Variant& val, uint32_t idx)
    {
        auto it = m_child_param_idx.find(idx);
        if (it != m_child_param_idx.end())
        {
            m_processors.at(it->second)->SetParam(val);
        }
    }

protected:
    std::vector<std::shared_ptr<Processor>> m_processors;

private:
    // Keep track of location of sub-processor parameters in a map
    // from param index -> processor index
    std::unordered_map<uint32_t, uint32_t> m_child_param_idx;
    std::vector<cppbase::Variant> m_params;

    template<class Archive>
    void serialize(Archive& archive, const uint32_t version)
    {
        SERIALIZE_BASE_CLASS(archive, Processor);
        archive(m_processors);
    }

    ENABLE_TYPE_INFO(Processor)
    SERIALIZATION_FRIEND_ACCESS
};

}} // namespaces
