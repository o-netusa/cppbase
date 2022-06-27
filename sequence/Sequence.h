/*****************************************************************************
 * @file: Sequence.hpp
 * @brief: 定义执行序列基类
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 * **************************************************************************/

#pragma once

#include <common/Timer.h>

#include <taskflow/taskflow.hpp>
#include <unordered_map>
#include <vector>

#include "Container.h"
#include "Link.h"

namespace cppbase { namespace sequence {

class Sequence : public Container
{
public:
    struct SequenceExecutionStatus : public Processor::ExecutionStatus
    {
        uint64_t trigger_count{0};
        uint64_t exec_count{0};
        uint64_t ok_count{0};
        uint64_t ng_count{0};

        SequenceExecutionStatus& operator=(const Processor::ExecutionStatus& status)
        {
            id = status.id;
            exec_status = status.exec_status;
            err_msg = status.err_msg;
            exec_time_us = status.exec_time_us;
            return *this;
        }
    };

public:
    Sequence() = default;
    virtual ~Sequence()
    {
        if (m_exec_thread.joinable())
        {
            m_mode = Mode::PROGRAM;
            m_exec_thread.join();
        }
    }

    template <typename T>
    Processor* CreateProcessor()
    {
        m_processors.push_back(std::make_shared<T>());
        return m_processors.back().get();
    }

    const SequenceExecutionStatus& GetExecutionStatus() const { return m_exec_status; }

    /**
     * @brief Add a link from src processor to dst processor
     */
    void AddLink(Processor* src, Processor* dst, uint32_t out_id = 0, uint32_t in_id = 0)
    {
        if (!src || !dst)
        {
            throw std::invalid_argument("Source or destination processor is null");
        }
        AddLink(src->GetId(), dst->GetId(), out_id, in_id);
    }

    /**
     * Add a link from src processor to dst processor
     */
    void AddLink(uuids::uuid src, uuids::uuid dst, uint32_t out_id = 0, uint32_t in_id = 0)
    {
        if (src.is_nil() || dst.is_nil())
        {
            throw std::invalid_argument("Source or destination processor is null");
        }
        if (m_proc_links.find(src) == m_proc_links.end())
        {
            std::vector<Link> links;
            links.emplace_back(src, dst, out_id, in_id);
            m_proc_links.emplace(src, links);
        } else
        {
            bool exists = false;
            auto& links = m_proc_links[src];
            Link new_link{src, dst, out_id, in_id};
            for (auto& link : links)
            {
                if (link == new_link)
                {
                    exists = true;
                    break;
                }
            }
            if (!exists)
                links.push_back(new_link);
        }
    }

    void RemoveLink(uuids::uuid src, uuids::uuid dst = uuids::uuid(), uint32_t out_id = 0,
                    uint32_t in_id = 0)
    {
        if (src.is_nil())
        {
            throw std::invalid_argument("Source processor must not be null");
        }
        if (m_proc_links.find(src) == m_proc_links.end())
        {
            throw std::out_of_range("No link starting with specified source processor");
        }
        auto& links = m_proc_links[src];
        Link new_link{src, dst, out_id, in_id};
        links.erase(std::remove_if(links.begin(), links.end(), [&new_link](auto& link) {
            if (new_link.m_src == link.m_src)
            {
                if (new_link.m_dst.is_nil())  // if m_dst is nullptr, remove all links with same m_src
                    return true;
                return new_link == link;
            }
        }));
    }

    ExecutionStatus Execute(const std::vector<cppbase::Variant>& inputs) override
    {
        std::vector<cppbase::Variant> seq_results;
        std::unordered_map<uuids::uuid, std::vector<cppbase::Variant>> proc_inputs_map;
        std::unordered_map<uuids::uuid, tf::Task> proc_task_map;
        tf::Executor executor{1};
        tf::Taskflow taskflow{uuids::to_string(m_id)};
        std::vector<std::future<void>> futures;
        ExecStatus exec_status{ExecStatus::PASS};

        m_exec_status.trigger_count++;

        if (!PreExecute(inputs))
        {
            return m_exec_status;
        }

        for (auto& proc : m_processors)
        {
            auto proc_id = proc->GetId();
            // Initialize processor's inputs with sequence's inputs at the beginning, based on
            // the processor input map in the sequence. The inputs may be overwriten during the
            // execution if there are links linked into the processor.
            proc_inputs_map.emplace(proc_id, std::vector<cppbase::Variant>(proc->GetInputTypes().size()));
            if (m_proc_inputs.find(proc_id) != m_proc_inputs.end())
            {
                auto& proc_inputs = m_proc_inputs[proc_id];
                for (auto& input : proc_inputs)
                {
                    proc_inputs_map[proc_id][input.second] = inputs[input.first];
                }
            }

            // Add a task for each processor to the task map
            proc_task_map.emplace(proc_id, taskflow.emplace([this, proc_id, &proc_inputs_map,
                                                             &exec_status, &seq_results]() {

                auto* proc = const_cast<Processor*>(GetProcessor(proc_id));
                try
                {
                    // Get the inputs for the processor and execute
                    auto& proc_inputs = proc_inputs_map[proc_id];
                    auto status = proc->Execute(proc_inputs);
                    if (status.exec_status == ExecStatus::PASS)
                    {
                        const auto& results = proc->GetResults();

                        // Update the successor's inputs if there're links between the current processor
                        // and its successor.
                        auto it = m_proc_links.find(proc_id);
                        if (it != m_proc_links.end())
                        {
                            for (auto& link : it->second)
                            {
                                if (!link.m_dst.is_nil())
                                {
                                    if (link.m_src_id >= results.size())
                                    {
                                        std::cerr
                                            << "Link source id is out of results range, source_id = "
                                            << link.m_src_id << " results.size() = " << results.size()
                                            << std::endl;
                                        throw std::out_of_range(
                                            "Link source id is out of results range");
                                    }
                                    auto& dst_inputs = proc_inputs_map[link.m_dst];
                                    if (dst_inputs.size() == link.m_src_id)
                                    {
                                        proc_inputs_map[link.m_dst].push_back(results[link.m_src_id]);
                                    } else if (dst_inputs.size() > link.m_src_id)
                                    {
                                        proc_inputs_map[link.m_dst][link.m_dst_id] =
                                            results[link.m_src_id];
                                    }
                                }
                            }
                        }
                    }
                } catch (...)
                {
                    exec_status = ExecStatus::FAIL;
                }
                seq_results.emplace_back(proc->GetExecutionStatus());
            }));
        }

        // Sort the processors according to the links
        for (auto element : m_proc_links)
        {
            auto& task = proc_task_map[element.first];
            for (auto& link : m_proc_links[element.first])
            {
                if (!link.m_dst.is_nil())
                {
                    task.precede(proc_task_map[link.m_dst]);
                }
            }
        }

        // Now run the taskflow which will run each added task
        auto future = executor.run(taskflow);
        future.wait();

        PostExecute(seq_results);
        {
            std::lock_guard<std::mutex> lock(m_results_mutex);
            m_results = seq_results;
        }

        m_exec_status = Processor::GetExecutionStatus();
        m_exec_status.exec_count++;
        m_exec_status.exec_status = exec_status;
        if (m_exec_status.exec_status == ExecStatus::PASS)
        {
            m_exec_status.ok_count++;
        } else if (m_exec_status.exec_status == ExecStatus::FAIL)
        {
            m_exec_status.ng_count++;
        }

        for (auto& callback : m_on_complete_callbacks)
        {
            futures.push_back(std::async(std::launch::async, callback, m_results));
        }

        return m_exec_status;
    }

    void SetMode(Mode mode) override
    {
        if (m_mode == mode)
            return;

        std::lock_guard<std::recursive_mutex> lock(m_exec_mutex);
        m_mode = mode;

        for (auto& proc : m_processors)
        {
            proc->SetMode(m_mode);
        }
        if (m_mode == Mode::RUN || m_mode == Mode::TEST)
        {
            m_exec_thread = std::thread([this]() {
                while (m_mode == Mode::RUN || m_mode == Mode::TEST)
                {
                    Execute({});
                }
            });
        } else
        {
            m_exec_thread.join();
        }
        std::vector<std::future<void>> futures;
        for (auto& callback : m_on_mode_changed_callbacks)
        {
            futures.push_back(std::async(std::launch::async, callback, m_mode));
        }
    }

    std::vector<const Processor*> GetSuccessors(const uuids::uuid& proc_id) const
    {
        std::vector<const Processor*> successors;
        auto it = m_proc_links.find(proc_id);
        if (it != m_proc_links.end())
        {
            for (auto& link : it->second)
            {
                if (!link.m_dst.is_nil())
                {
                    successors.push_back(GetProcessor(link.m_dst));
                }
            }
        }
        return successors;
    }

    std::vector<const Processor*> GetPredecessors(const uuids::uuid& proc_id) const
    {
        std::vector<const Processor*> predecessors;
        for (auto& element : m_proc_links)
        {
            for (auto& link : element.second)
            {
                if (link.m_dst == proc_id)
                {
                    predecessors.push_back(GetProcessor(link.m_src));
                }
            }
        }
        return predecessors;
    }

    std::vector<const Link*> GetLinks(const Processor* proc) const
    {
        std::vector<const Link*> links;
        auto it = m_proc_links.find(proc->GetId());
        if (it != m_proc_links.end())
        {
            for (auto& link : it->second)
            {
                links.push_back(&link);
            }
        }
        return links;
    }

    const Link* GetLink(const uuids::uuid& src_id, const uuids::uuid& dst_id)
    {
        auto it = m_proc_links.find(src_id);
        if (it != m_proc_links.end())
        {
            for (auto& link : it->second)
            {
                if (link.m_dst == dst_id)
                {
                    return &link;
                }
            }
        }
        return nullptr;
    }

    void ResetExecutionStatus() { m_exec_status = SequenceExecutionStatus(); }

    void MapProcessorInput(const uuids::uuid& proc_id, uint32_t seq_input_id, uint32_t proc_input_id)
    {
        if (m_proc_inputs.find(proc_id) == m_proc_inputs.end())
        {
            m_proc_inputs[proc_id] = std::vector<std::pair<uint32_t, uint32_t>>();
        }
        m_proc_inputs[proc_id].push_back(std::make_pair(seq_input_id, proc_input_id));
    }

protected:
    // maps from a processor to links which have the key as the source processor
    std::unordered_map<uuids::uuid, std::vector<Link>> m_proc_links;
    std::thread m_exec_thread;
    SequenceExecutionStatus m_exec_status;
    std::unordered_map<uuids::uuid, std::vector<std::pair<uint32_t, uint32_t>>> m_proc_inputs;

    template<class Archive>
    void serialize(Archive& archive, const uint32_t)
    {
        SERIALIZE_BASE_CLASS(archive, Container);
        archive(m_proc_links, m_proc_inputs);
    }

    ENABLE_TYPE_INFO(Container)
    SERIALIZATION_FRIEND_ACCESS
};

}}  // namespace ovf2::sequence
