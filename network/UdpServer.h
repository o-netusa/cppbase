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

class UdpServer
{
public:
    UdpServer(uint16_t port_num)
        : m_socket(network::io_context, udp::endpoint(udp::v4(), port_num))
    {}
    ~UdpServer()
    {
        Stop();
    }

    void Start(std::function<void(uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint)> receive_handler)
    {
        if (m_started)
            return;
        m_receive_handler = receive_handler;
        m_receive_future = std::async(std::launch::async, [this](){ StartReceive(); });
        m_started = true;
    }

    void Stop()
    {
        if (!m_started)
            return;
        m_socket.close();
        m_started = false;
        m_receive_future.wait();
    }

    uint32_t Send(const uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint)
    {
        uint32_t ret = 0;
        if (!buffer || bufsz == 0)
        {
            network::logger->error("UdpServier::Send: invalid input buffer");
            return ret;
        }
        return m_socket.send_to(asio::buffer(buffer, bufsz), endpoint);
    }

    uint32_t Receive(uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint)
    {
        uint32_t ret = 0;
        if (!buffer || bufsz == 0)
        {
            network::logger->error("UdpServier::Receive: invalid input buffer");
            return ret;
        }
        return m_socket.receive_from(asio::buffer(buffer, bufsz), endpoint);
    }

private:
    void StartReceive()
    {
        memset(m_buffer, 0, DEFAULT_BUFSZ);
        udp::endpoint client_endpoint;
        auto size = m_socket.receive_from(asio::buffer(m_buffer, DEFAULT_BUFSZ), client_endpoint);
        if (!m_started)
            return;
        if (size > 0)
            m_receive_handler(m_buffer, size, client_endpoint);
        if (!m_started)
            return;
        StartReceive();
    }
private:
    udp::socket m_socket;
    std::function<void(uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint)> m_receive_handler;
    std::future<void> m_receive_future;
    bool m_started{false};
    uint8_t m_buffer[DEFAULT_BUFSZ];
};

}
