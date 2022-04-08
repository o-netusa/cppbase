/**************************************************************************
 * @file:  TcpClient.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include "Common.h"

namespace cppbase {

/**
 * @brief TCP Client class
 */
class TcpClient
{
public:
    TcpClient() = default;
    ~TcpClient() { Disconnect(); }

    bool Connect(const std::string& ip_addr, uint16_t port_num)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        asio::error_code ec;
        if (m_is_connected)
        {
            network::logger->error("TcpClient::Connect: already connected.");
            return false;
        }
        auto address = asio::ip::address::from_string(ip_addr.c_str(), ec);
        if (ec)
        {
            network::logger->error("TcpClient::Connect: invalid ip address({}): {}", ip_addr,
                                   ec.message());
            return false;
        }
        m_sock.connect(tcp::endpoint(address, port_num), ec);
        if (ec)
        {
            network::logger->error("TcpClient::Connect: connect to {}:{} error: ", ip_addr,
                                   port_num, ec.message());
            return false;
        }

        m_is_connected = true;

        return true;
    }

    void Disconnect()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_is_connected)
        {
            asio::error_code ec;
            m_sock.close(ec);
            if (ec)
            {
                network::logger->error("TcpClient::Disconnect: error closing socket: {}",
                                       ec.message());
            }
        }
        m_is_connected = false;
    }

    uint32_t Send(const uint8_t* buffer, uint32_t size)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        uint32_t ret = 0;

        if (!buffer || size == 0 || !m_is_connected)
            return ret;

        asio::error_code ec;
        ret = m_sock.send(asio::buffer(buffer, size), 0, ec);
        if (ec)
            network::logger->error("TcpClient::Send: error sending buffer: {}", ec.message());

        return ret;
    }

    uint32_t Receive(uint8_t* buffer, uint32_t size)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        uint32_t ret = 0;

        if (!buffer || size == 0 || !m_is_connected)
            return ret;

        asio::error_code ec;
        ret = m_sock.receive(asio::buffer(buffer, size), 0, ec);
        if (ec)
            network::logger->error("TcpClient::Receive: error receiving buffer: {}", ec.message());

        return ret;
    }

    bool IsOpen() const
    {
        return m_is_connected;
    }

protected:
    tcp::socket m_sock{network::context.io_context};
    std::atomic<bool> m_is_connected{false};
    std::mutex m_mutex;
};

}  // namespace cppbase
