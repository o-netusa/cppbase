/**************************************************************************
 * @file:  LoggingTests.cpp
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
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
