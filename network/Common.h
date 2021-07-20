/**************************************************************************
 * @file:  Common.h
 * @brief:
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

namespace cppbase {
namespace network {

static logging::LoggerPtr logger{logging::GetLoggerForCurrentModule()};
static asio::io_context io_context;
// executor_work_guard prevents io_context from returning
static asio::executor_work_guard<asio::io_context::executor_type> work_guard{
    asio::make_work_guard(io_context)};
// io_thread keeps io_context running
static std::thread io_thread;

static void Init()
{
    io_thread = std::thread([] { io_context.run(); });
}

static void Terminate()
{
    io_context.stop();
    io_thread.join();
}

}  // namespace network

using tcp = asio::ip::tcp;
using udp = asio::ip::udp;

}  // namespace cppbase
