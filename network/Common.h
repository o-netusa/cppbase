/**************************************************************************
 * @file:  Common.h
 * @brief: Network common header
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <common/Global.h>
#include <logging/Logging.h>

#include <asio.hpp>
#include <future>
#include <thread>

#define DEFAULT_BUFSZ 4096

namespace cppbase { namespace network {

static logging::LoggerPtr logger{logging::GetLoggerForCurrentModule()};

struct Context
{
    Context()
    {
        io_thread = std::thread([this] { io_context.run(); });
    }

    ~Context()
    {
        io_context.stop();
        io_thread.join();
    }

    asio::io_context io_context;
    // executor_work_guard prevents io_context from returning
    asio::executor_work_guard<asio::io_context::executor_type> work_guard{
        asio::make_work_guard(io_context)};
    // io_thread keeps io_context running
    std::thread io_thread;
};

static asio::io_context& get_io_context()
{
    static Context context{};
    return context.io_context;
}

} // namespace network

using tcp = asio::ip::tcp;
using udp = asio::ip::udp;
using address = asio::ip::address;

}  // namespace cppbase
