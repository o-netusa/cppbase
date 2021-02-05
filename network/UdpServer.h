/**************************************************************************
 * @file:  UdpServer.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <functional>
#include "Common.h"

namespace cppbase {

class DLLEXPORT UdpServer
{
public:
    UdpServer(uint16_t port_num)
        : m_io_context(GetIOContext()),
          m_socket(GetIOContext(), udp::endpoint(udp::v4(), port_num))
    {}

    void Start(std::function<void()> receive_handler)
    {
        if (m_started)
            return;
        m_receive_handler = receive_handler;
        StartReceive();
        m_started = true;
    }

    void Stop()
    {
        if (!m_started)
            return;
        m_socket.close();
        m_started = false;
    }

private:
    void StartReceive()
    {
    }
private:
    asio::io_context& m_io_context;
    udp::socket m_socket;
    udp::endpoint m_remote_endpoint;
    std::function<void()> m_receive_handler;
    bool m_started{false};
};

}
