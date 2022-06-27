/**************************************************************************
 * @file:  Logging.h
 * @brief: spdlog based Logger
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <common/Global.h>
#include <spdlog/spdlog.h>

namespace cppbase { namespace logging {

typedef std::shared_ptr<spdlog::logger> LoggerPtr;

/**
 * @brief Get the Logger For Current Module object
 * @note By default, each lib should have a module name defined, and the name
 *   will be used for the logger
 * @return LoggerPtr
 */
static LoggerPtr GetLoggerForCurrentModule() noexcept;

}}  // namespace cppbase::logging

#include "Logging_intl.h"
