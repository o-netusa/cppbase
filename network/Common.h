/**************************************************************************
 * @file:  Common.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <future>
#include <thread>

#include <asio.hpp>
#include <common/Global.h>
#include <logging/Logging.h>

namespace cppbase {

class DLLEXPORT NetworkBase
{
public:
    inline static logging::LoggerPtr logger{logging::GetLoggerForCurrentModule()};

    NetworkBase()
    {}

    ~NetworkBase()
    {
        m_io_context.stop();
        m_thread.join();
    }

    asio::io_context& GetIOContext()
    {
        return m_io_context;
    }

protected:
    asio::io_context m_io_context{};
    asio::executor_work_guard<asio::io_context::executor_type> m_work_guard
    { asio::make_work_guard(m_io_context) };
    std::thread m_thread{ [this]{ m_io_context.run(); } };
};

}

using tcp = asio::ip::tcp;
using udp = asio::ip::udp;
