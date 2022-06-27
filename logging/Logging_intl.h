/**************************************************************************
 * @file:  Logging_intl.h
 * @brief: Logging internal implementations
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
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
        static Instance m_instance;
        return m_instance;
    }

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
     *   But the configuration can be changed in the \b log.conf file.
     */
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
                try
                {
                    // TODO: for debug only
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
