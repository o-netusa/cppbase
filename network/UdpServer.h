/**************************************************************************
 * @file:  UdpServer.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <atomic>
#include <functional>
#include <mutex>

#include "Common.h"

namespace cppbase {

/**
 * @brief UDP Server class
 */
class UdpServer
{
public:
    UdpServer(const std::string& ip, uint16_t port_num)
        : m_socket(network::get_io_context(), udp::endpoint(address::from_string(ip), port_num))
    {}
    ~UdpServer() { Stop(); }

    void Start(std::function<void(uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint)>
                   receive_handler)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_started)
            return;
        m_receive_handler = receive_handler;
        m_receive_future = std::async(std::launch::async, [this]() { StartReceive(); });
        m_started = true;
    }

    void Stop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_socket.is_open())
            return;
        asio::error_code ec;
        m_socket.shutdown(asio::socket_base::shutdown_both, ec);
        if (ec)
        {
            network::logger->error("UdpServer::Stop: error shutting down socket: {}", ec.message());
        }
        m_socket.close(ec);
        if (ec)
        {
            network::logger->error("UdpServer::Stop: error closing socket: {}", ec.message());
        }
        if (m_started)
        {
            m_started = false;
            m_receive_future.wait();
        }
    }

    uint32_t Send(const uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint)
    {
        uint32_t ret = 0;
        if (!buffer || bufsz == 0)
        {
            network::logger->error("UdpServier::Send: invalid input buffer");
            return ret;
        }
        std::lock_guard<std::mutex> lock(m_mutex);
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
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_socket.receive_from(asio::buffer(buffer, bufsz), endpoint);
    }

    bool IsOpen() { return m_started; }

protected:
    void StartReceive()
    {
        if (!m_started)
            return;

        memset(m_buffer, 0, DEFAULT_BUFSZ);
        udp::endpoint client_endpoint;
        auto size = m_socket.receive_from(asio::buffer(m_buffer, DEFAULT_BUFSZ), client_endpoint);
        if (!m_started)
            return;
        if (size > 0)
            m_receive_handler(m_buffer, size, client_endpoint);
        StartReceive();
    }

protected:
    udp::socket m_socket;
    std::function<void(uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint)> m_receive_handler;
    std::future<void> m_receive_future;
    std::atomic<bool> m_started{false};
    uint8_t m_buffer[DEFAULT_BUFSZ];
    std::mutex m_mutex;
};

}  // namespace cppbase
