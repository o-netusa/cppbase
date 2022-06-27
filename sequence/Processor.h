/*****************************************************************************
 * @file: Processor.h
 * @brief: 处理单元基类，该类是序列中的最小可执行单元。
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 * **************************************************************************/

#pragma once

#include <common/Archive.h>
#include <common/PropertyPath.h>
#include <common/Timer.h>
#include <common/Uuid.h>
#include <common/Variant.h>

#include <atomic>
#include <future>
#include <memory>

namespace cppbase { namespace sequence {

/**
 * Processor Class - Base class for all processors
 * A Processor is a minimum executable unit in a Sequence. All processors can have parameters and
 * inputs, and generate outputs. Parameters should be passed into Initialize() during the
 * initialization time. Inputs can be configured in Initialize() and during runtime by calling
 * AddInput() and RemoveInput(). Execute() returns a vector of cppbase::Variants, which represent
 * the results.
 */
class Processor
{
public:
    /**
     * @brief: Processor execution mode
     *   PROGRAM - the processor is in program mode
     *   RUN     - the processor is in run mode
     *   TEST    - the processor is in test mode
     */
    enum class Mode : int
    {
        PROGRAM = 0x1,
        RUN = 0x2,
        TEST = 0x4
    };

    enum class ExecStatus : int
    {
        SKIPPED = -1,
        PASS = 0,
        FAIL = 1
    };

    struct ExecutionStatus
    {
        uuids::uuid id;
        ExecStatus exec_status{ExecStatus::SKIPPED};
        std::string err_msg;
        double exec_time_us{0};
    };

public:
    DISALLOW_COPY_AND_ASSIGN(Processor);

    Processor() = default;
    virtual ~Processor() = default;

    /**
     * @brief Get the processor's ID
     *
     * @return const uuids::uuid&
     */
    const uuids::uuid& GetId() const { return m_id; }

    /**
     * @brief Set the function that returns if the processor is executable
     */
    void SetExecutableFunc(std::function<bool(void)> func) { m_executable_func = func; }

    /**
     * @brief Get the function that returns if the processor is executable
     *
     * @return std::function<bool(void)>
     */
    std::function<bool(void)> GetExecutableFunc() const { return m_executable_func; }

    /**
     * @brief Get a boolean that indicates if the processor is executable
     */
    bool GetExecutable() { return m_executable_func(); }

    /**
     * @brief Set the processor's parent
     *
     * @param parent
     */
    void SetParent(Processor* parent) { m_parent = parent; }

    /**
     * @brief Get the processor's parent
     *
     * @return Processor* Point to the parent processor
     */
    Processor* GetParent() const { return m_parent; }

    /**
     * @brief Get the processor's name
     *
     * @return const std::string&
     */
    const std::string& GetName() const { return m_name; }

    /**
     * @brief Set the processor's name
     *
     * @param name
     */
    void SetName(const std::string& name) { m_name = name; }

    /**
     * @brief Get the processor's input types
     *
     * @return vector of input name and types
     */
    std::vector<std::pair<std::string, cppbase::Variant::Type>> GetInputTypes() const
    {
        std::vector<std::pair<std::string, cppbase::Variant::Type>> types;
        for (auto& input : m_inputs)
        {
            types.push_back({input.first, input.second.GetType()});
        }
        return types;
    }

    /**
     * @brief Get the processor's output types
     *
     * @return vector of output name and types
     */
    std::vector<std::pair<std::string, cppbase::Variant::Type>> GetOutputTypes() const
    {
        std::vector<std::pair<std::string, cppbase::Variant::Type>> types;
        for (auto& output : m_outputs)
        {
            types.push_back({output.first, output.second.GetType()});
        }
        return types;
    }

    /**
     * @brief Check if the passed in inputs match the processor's input types
     *
     * @return true if the inputs match, otherwise false
     */
    bool CheckInputType(const std::vector<cppbase::Variant>& inputs)
    {
        if (inputs.size() > m_inputs.size())
            return false;

        for (auto input : inputs)
        {
            std::vector<std::pair<std::string, cppbase::PropertyPath>>::iterator it = std::find_if(
                m_inputs.begin(), m_inputs.end(),
                [&input](const std::pair<std::string, cppbase::PropertyPath>& element) {
                    return element.second.GetType() == input.GetType() ||
                           element.second.GetRootType() == input.GetType();
                });

            if (it == m_inputs.end())
                return false;
        }
        return true;
    }

    /**
     * @brief Get the Execution Status object
     *
     * @return ExecutionStatus
     */
    const ExecutionStatus& GetExecutionStatus() const { return m_exec_status; }

    /**
     * @brief Register on complete callback
     * @note  Do NOT call GUI functions from this callback
     * @param callback
     */
    virtual void AddOnCompleteCallback(
        const std::function<void(const std::vector<cppbase::Variant>&)>& callback)
    {
        m_on_complete_callbacks.push_back(callback);
    }
    virtual void ClearOnCompleteCallback() { m_on_complete_callbacks.clear(); }

    /**
     * @brief Register on error callback
     * @note  Do NOT call GUI functions from this callback
     * @param callback
     */
    virtual void AddOnErrorCallback(const std::function<void(const std::string&)>& callback)
    {
        m_on_error_callbacks.push_back(callback);
    }
    virtual void ClearOnErrorCallback() { m_on_error_callbacks.clear(); }

    /**
     * @brief Register mode change callback
     * @note  Do NOT call GUI functions from this callback
     *
     * @param callback
     */
    virtual void AddOnModeChangedCallback(const std::function<void(Mode)>& callback)
    {
        m_on_mode_changed_callbacks.push_back(callback);
    }
    virtual void ClearOnModeChangedCallback() { m_on_mode_changed_callbacks.clear(); }

    /**
     * @brief Add an new input to the processor
     *   The type specifies the type of the input and the prop specifies the property
     *   of the processor's parameter that the input is bound to.
     *
     * @param name name of the input
     * @param type variant type of the input
     * @param path property path of the input
     * @return the input id
     */
    virtual uint32_t AddInput(const std::string& name, cppbase::Variant::Type type,
                              const std::string& path = "")
    {
        for (uint32_t idx = 0; idx < m_inputs.size(); ++idx)
        {
            if (m_inputs[idx].first == name)
            {
                return idx;
            }
        }
        m_inputs.emplace_back(name, cppbase::PropertyPath{type, path});
        return m_inputs.size() - 1;
    }

    virtual void RemoveInput(const std::string& name)
    {
        for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it)
        {
            if (it->first == name)
            {
                m_inputs.erase(it);
                break;
            }
        }
    }

    /**
     * @brief Add an new output to the processor
     *   The type specifies the type of the output and the prop specifies the property
     *   of the processor's parameter that the output is bound to.
     *
     * @param name name of the output
     * @param type variant type of the output
     * @param path property path of the output
     * @return the output id
     */
    virtual uint32_t AddOutput(const std::string& name, cppbase::Variant::Type type,
                               const std::string& path = "")
    {
        for (uint32_t idx = 0; idx < m_outputs.size(); ++idx)
        {
            if (m_outputs[idx].first == name)
            {
                return idx;
            }
        }
        m_outputs.emplace_back(name, cppbase::PropertyPath{type, path});
        return m_outputs.size() - 1;
    }

    virtual void RemoveOutput(const std::string& name)
    {
        for (auto it = m_outputs.begin(); it != m_outputs.end(); ++it)
        {
            if (it->first == name)
            {
                m_outputs.erase(it);
                break;
            }
        }
    }

    /**
     * @brief: Set parameter of the processor
     * @param param: value of the parameter
     */
    virtual void SetParam(const cppbase::Variant& param) { m_param = param; }

    /**
     * @brief: Get parameter of the processor
     * @return: value of the parameter
     */
    virtual const cppbase::Variant& GetParam() const { return m_param; }

    /**
     * @brief: Get the current mode of the processor
     * @return: Mode
     */
    Mode GetMode() const { return m_mode; }

    /**
     * @brief: Set the current mode of the processor
     * @param mode: Mode
     */
    virtual void SetMode(Mode mode)
    {
        if (m_mode == mode)
            return;

        std::lock_guard<std::recursive_mutex> lock(m_exec_mutex);
        m_mode = mode;
        std::vector<std::future<void>> futures;
        for (auto& callback : m_on_mode_changed_callbacks)
        {
            futures.push_back(std::async(std::launch::async, callback, mode));
        }
    }

    /**
     * @brief: Get the results of the processor's execution
     * @return: std::vector<cppbase::Variant>
     * @note: return by value on purpose. This function could be called by
     *   the GUI thread, while the execution thread could be updating m_results.
     *   Returning by value can prevent the race condition. The caller should
     *   obtain a const reference to the returned object.
     */
    virtual std::vector<cppbase::Variant> GetResults() const
    {
        std::lock_guard<std::mutex> lock(m_results_mutex);
        return m_results;
    }

    /**
     * @brief: Initialize the processor with the specified parameters
     * @param param: parameter
     * @return: true if the initialization is successful, otherwise false
     */
    virtual bool Initialize(const cppbase::Variant& param = {})
    {
        m_param = param;

        m_inputs.clear();
        m_outputs.clear();
        m_on_complete_callbacks.clear();
        m_on_error_callbacks.clear();
        m_on_mode_changed_callbacks.clear();

        return true;
    }

    /**
     * @brief: Execute the processor
     * @param inputs: inputs
     * @return: ExecutionStatus
     */
    virtual ExecutionStatus Execute(const std::vector<cppbase::Variant>& inputs) = 0;

protected:
    /**
     * @brief Check input type and set the input type of the algorithm
     * @param inputs - input data
     * @return true if the input type is correct, otherwise false
     * @note:
     * - Check the input type of the input data
     * - If the input type is not the same as the specified input type, return false
     */
    virtual bool PreExecute(const std::vector<cppbase::Variant>& inputs)
    {
        m_exec_status.id = m_id;
        m_exec_status.exec_status = ExecStatus::SKIPPED;
        m_exec_status.err_msg = "";
        m_exec_status.exec_time_us = 0;

        // if the processor is not executable, clear results and return false;
        if (!GetExecutable())
        {
            std::lock_guard<std::mutex> lock(m_results_mutex);
            m_results = {};
            return false;
        }
        if (!CheckInputType(inputs))
        {
            throw std::runtime_error("Input type doesn't match");
        }
        m_exec_timer.Start();
        return true;
    }
    /**
     * @brief Set the output data, which contains the user specified link outputs
     * @param outputs - output data
     * @param status  - execution status
     * @return true if the output data is set correctly, otherwise false
     * @note:
     * - Set the output data
     */
    virtual bool PostExecute(std::vector<cppbase::Variant>& outputs,
                             ExecStatus status = ExecStatus::PASS)
    {
        m_exec_status.exec_status = status;
        m_exec_status.exec_time_us = static_cast<double>(m_exec_timer.Elapsed());
        return true;
    }

protected:
    uuids::uuid m_id{cppbase::Uuid::Generate()};
    std::string m_name{"Processor"};
    Mode m_mode{Mode::PROGRAM};
    std::function<bool(void)> m_executable_func{[] { return true; }};
    Processor* m_parent{nullptr};
    std::vector<std::pair<std::string, cppbase::PropertyPath>> m_inputs;
    std::vector<std::pair<std::string, cppbase::PropertyPath>> m_outputs;
    cppbase::Variant m_param;
    std::vector<cppbase::Variant> m_results;
    mutable std::mutex m_results_mutex;
    mutable std::recursive_mutex m_exec_mutex;
    cppbase::TimerUs m_exec_timer;
    ExecutionStatus m_exec_status;

    std::vector<std::function<void(const std::vector<cppbase::Variant>&)>> m_on_complete_callbacks;
    std::vector<std::function<void(const std::string&)>> m_on_error_callbacks;
    std::vector<std::function<void(Mode)>> m_on_mode_changed_callbacks;

    template <class Archive>
    void serialize(Archive& archive, const uint32_t)
    {
        // TODO: m_executable_func and other callbacks
        archive(m_id, m_name, m_inputs, m_outputs);
    }

    ENABLE_TYPE_INFO()
    SERIALIZATION_FRIEND_ACCESS
};

}}  // namespace ovf2::sequence
