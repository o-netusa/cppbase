/**************************************************************************
 * @file: TestProcessors.h
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <common/Archive.h>
#include <sequence/Processor.h>

namespace cppbase { namespace sequence {

class BinaryOpProcessor : public Processor
{
public:
    enum class Operator : int
    {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
    };

    BinaryOpProcessor() = default;
    BinaryOpProcessor(Operator op) : m_op(op) {}
    ~BinaryOpProcessor() override = default;

    bool Initialize(const cppbase::Variant& param = {}) override
    {
        Processor::Initialize(param);

        AddInput("input0", cppbase::Variant::GetType<float>());
        AddInput("input1", cppbase::Variant::GetType<float>());

        return true;
    }

    ExecutionStatus Execute(const std::vector<cppbase::Variant>& inputs) override
    {
        if (!PreExecute(inputs))
        {
            return m_exec_status;
        }
        float result;
        float input0 = inputs[0].GetValue<float>();
        float input1 = inputs[1].GetValue<float>();
        switch (m_op)
        {
        case Operator::ADD:
            result = input0 + input1;
            break;
        case Operator::SUB:
            result = input0 - input1;
            break;
        case Operator::MUL:
            result = input0 * input1;
            break;
        case Operator::DIV:
            result = input0 / input1;
            break;
        case Operator::MOD:
            result = std::fmod(input0, input1);
            break;
        default:
            throw std::runtime_error("BinaryOpProcessor: Invalid Operator");
        }
        m_results = {result};
        PostExecute(m_results);

        return m_exec_status;
    }

private:
    template<typename Archive>
    void save(Archive& ar, const uint32_t) const
    {
        SERIALIZE_VIRTUAL_BASE_CLASS(ar, Processor);
        ar(m_op);
    }

    template<typename Archive>
    void load(Archive& ar, const uint32_t)
    {
        SERIALIZE_VIRTUAL_BASE_CLASS(ar, Processor);
        ar(m_op);
        if (!Initialize())
        {
            throw std::runtime_error("BinaryOpProcessor: Deserialize initializaton failed");
        }
    }

    SERIALIZATION_FRIEND_ACCESS

private:
    Operator m_op;
};

}} // namespace cppbase::sequence

REGISTER_TYPE(cppbase::sequence::BinaryOpProcessor)
SPECIALIZE_SAVE_LOAD_ARCHIVE(cppbase::sequence::BinaryOpProcessor)
