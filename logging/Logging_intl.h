/**************************************************************************
 * @file:  Logging_intl.h
 * @brief:
 *
 * Copyright 2021 O-Net Technologies (Group) Limited.
**************************************************************************/

#include "spdlog_setup/conf.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <common/FileSystem.h>

namespace cppbase { namespace logging {

class Instance
{
public:
    static constexpr const char log_pattern[] = "[%Y-%m-%dT%T%z] [%L] <%n>: %v";

    Instance()
    {
        fs::path log_conf = fs::path(filesystem::GetConfigDir())/filesystem::LogConfigFilename;
        if (fs::exists(log_conf))
        {
            spdlog_setup::from_file(log_conf.generic_string());
        }
        else
        {
            spdlog::set_pattern(log_pattern);
        }
    }
};

static Instance instance;

LoggerPtr GetLogger(const std::string& logger_name) noexcept
{
    if (logger_name.empty()) // return the global logger
    {
        return spdlog::default_logger();
    }
    else
    {
        auto logger = spdlog::get(logger_name);
        if (!logger)
        {
            try 
            {
                // TODO: for debug only
                logger = spdlog::stdout_color_mt(logger_name.c_str());
            }
            catch (std::exception ex)
            {
                logger = spdlog::default_logger();
                logger->error("Error: unable to create logger with name: {0}, "
                              "using default logger", logger_name);
            }
        }
        return logger;
    }
}

[[maybe_unused]] LoggerPtr GetLoggerForCurrentModule() noexcept
{
#ifdef MODULE_NAME
    return GetLogger(MODULE_NAME);
#else
    #error "Error: MODULE_NAME is not defined. Define it in the module's CMakeLists.txt."
#endif
}

}} // namespaces
