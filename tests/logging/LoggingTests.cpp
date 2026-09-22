/**************************************************************************
 * @file:  LoggingTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 **************************************************************************/

#include <gtest/gtest.h>
#include <logging/Logging.h>

using namespace cppbase::logging;

TEST(LoggingTests, GetLoggers)
{
    auto default_logger = Instance::GetInstance().GetLogger();
    EXPECT_NE(nullptr, default_logger);
    default_logger->info("This is logged from default logger.");

    auto module_logger = GetLoggerForCurrentModule();
    EXPECT_NE(nullptr, module_logger);
    module_logger->info("This is logged from cppbase module logger.");
}
