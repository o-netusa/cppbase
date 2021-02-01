/**************************************************************************
 * @file:  LoggingTests.cpp
 * @brief:
 *
 * Copyright 2021 O-Net Technologies (Group) Limited.
**************************************************************************/

#include <gtest/gtest.h>
#include <logging/Logging.h>

using namespace cppbase::logging;

TEST(LoggingTests, GetLoggers)
{
    auto default_logger = GetLogger();
    EXPECT_NE(nullptr, default_logger);
    default_logger->info("This is logged from default logger.");

    auto module_logger = GetLoggerForCurrentModule();
    EXPECT_NE(nullptr, module_logger);
    module_logger->info("This is logged from cppbase module logger.");
}
