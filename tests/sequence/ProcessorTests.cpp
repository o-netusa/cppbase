/**************************************************************************
 * @file: ProcessorTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
 *************************************************************************/

#include <common/Serializer.h>
#include <gtest/gtest.h>

#include "TestProcessors.h"

using namespace cppbase::sequence;

TEST(ProcessorTests, Basic)
{
    BinaryOpProcessor proc(BinaryOpProcessor::Operator::ADD);
    proc.Initialize();
    auto input_types = proc.GetInputTypes();
    EXPECT_EQ(input_types[0].second, rttr::type::get<float>());
    EXPECT_EQ(input_types[1].second, rttr::type::get<float>());
    proc.AddInput("input2", rttr::type::get<int>());
    input_types = proc.GetInputTypes();
    EXPECT_EQ(input_types.size(), 3);
    EXPECT_EQ(input_types[2].second, rttr::type::get<int>());
    std::vector<cppbase::Variant> inputs{1.0f, 2.0f};
    EXPECT_TRUE(proc.CheckInputType(inputs));
    proc.RemoveInput("input2");
    input_types = proc.GetInputTypes();
    EXPECT_EQ(input_types.size(), 2);
    EXPECT_TRUE(proc.CheckInputType(inputs));
    EXPECT_NO_THROW(proc.Execute(inputs));
    const auto& results = proc.GetResults();
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].GetValue<float>(), 3.f);
}

TEST(ProcessorTests, Serialization)
{
    auto proc = std::make_shared<BinaryOpProcessor>(BinaryOpProcessor::Operator::ADD);
    proc->Initialize();

    auto proc_ser = cppbase::Serializer::SaveObjectToStringBinary(proc);
    auto proc2 = cppbase::Serializer::LoadObjectFromStringBinary<std::shared_ptr<BinaryOpProcessor>>(proc_ser);
    EXPECT_EQ(proc->GetInputTypes().size(), proc2->GetInputTypes().size());

    auto inputs = std::vector<cppbase::Variant>{1.0f, 2.0f};
    EXPECT_TRUE(proc2->CheckInputType(inputs));
    EXPECT_NO_THROW(proc2->Execute(inputs));
    const auto& results = proc2->GetResults();
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].GetValue<float>(), 3.f);
}
