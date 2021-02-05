/**************************************************************************
 * @file:  Common.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <future>

#include <asio.hpp>
#include <common/Global.h>
#include <logging/Logging.h>

namespace cppbase {

static asio::io_context& GetIOContext()
{
    static asio::io_context io_context;
    static std::thread t([&] { io_context.run(); });
    return io_context;
}

static auto logger = logging::GetLoggerForCurrentModule();

using tcp = asio::ip::tcp;
using udp = asio::ip::udp;

}
