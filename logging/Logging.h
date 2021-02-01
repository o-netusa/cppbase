/**************************************************************************
 * @file:  Logging.h
 * @brief:
 *
 * Copyright 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <spdlog/spdlog.h>

#include <common/Global.h>

namespace cppbase { namespace logging {

typedef std::shared_ptr<spdlog::logger> LoggerPtr;

/**
 * @brief GetLogger
 *   Get the specified logger. If \p logger_name is empty, the global logger
 *   will be returned. A logger will be created if the specified name cannot
 *   be found.
 *   Logging class is responsible for creating specialized loggers.
 *   Each module (.so or .dll) can get its own logger, as long as
 *   it has ONET_MODULE_NAME macro defined.
 * @param[in] logger_name
 * @return A spdlog logger
 * @note
 *   By default, in Debug mode, all logs will be logged into std::cout,
 *   while in Release mode, all logs will be logged into a log file.
 *   But the configuration can be changed in the log.conf file.
 */
static LoggerPtr GetLogger(const std::string& logger_name = "") noexcept;
static LoggerPtr GetLoggerForCurrentModule() noexcept;

}} // namespaces

#include "Logging_intl.h"
