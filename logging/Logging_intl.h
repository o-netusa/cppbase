/**************************************************************************
 * @file:  Logging_intl.h
 * @brief: Logging internal implementations
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 **************************************************************************/

#include <common/FileSystem.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "spdlog_setup/conf.h"

namespace cppbase { namespace logging {

class Instance
{
public:
    static constexpr const char log_pattern[] = "[%Y-%m-%dT%T%z] [%L] <%n>: %v";
    static Instance& GetInstance()
    {
        static Instance instance;
        return instance;
    }

    LoggerPtr GetLogger(const std::string& logger_name = "") noexcept
    {
        if (logger_name.empty())  // return the global logger
        {
            return spdlog::default_logger();
        } else
        {
            auto logger = spdlog::get(logger_name);
            if (!logger)
            {
                logger = spdlog::get("default");
                if (logger)
                {
                    return logger->clone(logger_name);
                }
                try
                {
                    // create a new logger if neighter "logger_name" nor "default" exist
                    logger = spdlog::stdout_color_mt(logger_name.c_str());
                } catch (std::exception ex)
                {
                    logger = spdlog::default_logger();
                    logger->error(
                        "Error: unable to create logger with name: {0}, "
                        "using default logger",
                        logger_name);
                }
            }
            return logger;
        }
    }

private:
    Instance()
    {
        fs::path log_conf = fs::path(filesystem::GetConfigDir()) / filesystem::LogConfigFilename;
        if (fs::exists(log_conf))
        {
            spdlog_setup::from_file(log_conf.generic_string());
        } else
        {
            spdlog::set_pattern(log_pattern);
        }
    }
};

[[maybe_unused]] LoggerPtr GetLoggerForCurrentModule() noexcept
{
#ifdef MODULE_NAME
    return Instance::GetInstance().GetLogger(MODULE_NAME);
#else
#error "Error: MODULE_NAME is not defined. Define it in the module's CMakeLists.txt."
#endif
}

}}  // namespace cppbase::logging
