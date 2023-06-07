/**************************************************************************
 * @file: SequenceTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#include <gtest/gtest.h>
#include <sequence/Sequence.h>

#include "TestProcessors.h"

using namespace cppbase::sequence;

TEST(SequenceTests, SequenceExecution)
{
    std::vector<cppbase::Variant> inputs{1.f, 2.f};

    auto A = std::make_shared<BinaryOpProcessor>(BinaryOpProcessor::Operator::ADD);
    A->Initialize();
    auto B = std::make_shared<BinaryOpProcessor>(BinaryOpProcessor::Operator::SUB);
    B->Initialize();
    auto C = std::make_shared<BinaryOpProcessor>(BinaryOpProcessor::Operator::MUL);
    C->Initialize();
    auto D = std::make_shared<BinaryOpProcessor>(BinaryOpProcessor::Operator::DIV);
    D->Initialize();

    Sequence S;
    S.AddProcessor(A);
    S.AddProcessor(B);
    S.AddProcessor(C);
    S.AddProcessor(D);

    // Map sequece inputs to A and B's inputs
    S.MapProcessorInput(A->GetId(), 0, 0);
    S.MapProcessorInput(A->GetId(), 1, 1);
    S.MapProcessorInput(B->GetId(), 0, 0);
    S.MapProcessorInput(B->GetId(), 1, 1);

    EXPECT_EQ(S.GetAllProcessors().size(), 4);
    EXPECT_EQ(A->GetParent(), &S);
    EXPECT_EQ(B->GetParent(), &S);
    EXPECT_EQ(C->GetParent(), &S);
    EXPECT_EQ(D->GetParent(), &S);
    EXPECT_TRUE(A->GetExecutable());
    EXPECT_TRUE(B->GetExecutable());
    EXPECT_TRUE(C->GetExecutable());
    EXPECT_TRUE(D->GetExecutable());

    /**
     * A (1.0 + 2.0) --+
     *                 |
     *                 +--->C (3.0 * -1.0) ---+
     *                 |                      |
     *                 |                      +--->D (-3.0 / -1.0)
     *                 |                      |
     * B (1.0 - 2.0) --+----------------------+
     */
    std::vector<float> actual_results{3.f, -1.f, -3.f, 3.f};
    S.AddLink(A.get(), C.get(), 0, 0);
    S.AddLink(B.get(), C.get(), 0, 1);
    S.AddLink(C.get(), D.get(), 0, 0);
    S.AddLink(B.get(), D.get(), 0, 1);

    S.AddInput("input0", cppbase::Variant::GetType<float>());
    S.AddInput("input1", cppbase::Variant::GetType<float>());
    EXPECT_NO_THROW(S.Execute(inputs));

    {
        const auto& results = S.GetResults();
        EXPECT_EQ(results.size(), 4);
        for (uint32_t i = 0; i < 4; ++i)
        {
            EXPECT_TRUE(results[i].IsType<Processor::ExecutionStatus>());
            auto& result = results[i].GetValue<Processor::ExecutionStatus>();
            ASSERT_EQ(result.exec_status, Processor::ExecStatus::PASS);
            const auto& proc_results = S.GetProcessor(result.id)->GetResults();
            ASSERT_EQ(proc_results.size(), 1);
            auto& proc_result = proc_results[0].GetValue<float>();
            EXPECT_FLOAT_EQ(proc_result, actual_results[i]);
        }

        const auto& status1 = S.GetExecutionStatus();
        EXPECT_EQ(status1.exec_status, Processor::ExecStatus::PASS);
        EXPECT_EQ(status1.trigger_count, 1);
        EXPECT_EQ(status1.exec_count, 1);
        EXPECT_EQ(status1.ok_count, 1);
        EXPECT_EQ(status1.ng_count, 0);
        std::cout << "Execution time: " << status1.exec_time_us << " us" << std::endl;
    }

    // Disable execution for A
    /**
     * A (not executed)+    sequence inputs[0]
     *                 |       V
     *                 +--->C (1.0 * -1.0) ---+
     *                 |                      |
     *                 |                      +--->D (-1.0 / -1.0)
     *                 |                      |
     * B (1.0 - 2.0) --+----------------------+
     */
    {
        actual_results = {nanf("don't care"), -1.f, -1.f, 1.f};
        A->SetExecutableFunc([] { return false; });
        S.MapProcessorInput(C->GetId(), 0, 0);
        EXPECT_NO_THROW(S.Execute(inputs));
        const auto& results = S.GetResults();
        EXPECT_EQ(results.size(), 4);
        for (uint32_t i = 0; i < 4; ++i)
        {
            EXPECT_TRUE(results[i].IsType<Processor::ExecutionStatus>());
            auto& result = results[i].GetValue<Processor::ExecutionStatus>();
            if (i == 0)
            {
                EXPECT_EQ(result.exec_status, Processor::ExecStatus::SKIPPED);
                const auto& proc_results = S.GetProcessor(result.id)->GetResults();
                EXPECT_EQ(proc_results.size(), 0);
            } else
            {
                EXPECT_EQ(result.exec_status, Processor::ExecStatus::PASS);
                const auto& proc_results = S.GetProcessor(result.id)->GetResults();
                EXPECT_EQ(proc_results.size(), 1);
                auto& proc_result = proc_results[0].GetValue<float>();
                EXPECT_FLOAT_EQ(proc_result, actual_results[i]);
            }
        }
    }

    const auto& status2 = S.GetExecutionStatus();
    EXPECT_EQ(status2.exec_status, Processor::ExecStatus::PASS);
    EXPECT_EQ(status2.trigger_count, 2);
    EXPECT_EQ(status2.exec_count, 2);
    EXPECT_EQ(status2.ok_count, 2);
    EXPECT_EQ(status2.ng_count, 0);
    std::cout << "Execution time: " << status2.exec_time_us << " us" << std::endl;
}