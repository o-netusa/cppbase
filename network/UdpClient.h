/**************************************************************************
 * @file:  UdpClient.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <atomic>
#include <mutex>

#include "Common.h"

namespace cppbase {

/**
 * @brief UDP Client class
 */
class UdpClient
{
public:
    UdpClient() : m_sock(network::context.io_context) {}
    UdpClient(uint16_t port_num)
        : m_sock(network::context.io_context, udp::endpoint(udp::v4(), port_num))
    {}
    ~UdpClient() { Disconnect(); }

    bool Connect(const std::string& ip_addr, uint16_t port_num)
    {
        asio::error_code ec;
        if (m_is_connected)
        {
            network::logger->error("UdpClient::Connect: already connected.");
            return false;
        }
        auto address = address::from_string(ip_addr.c_str(), ec);
        if (ec)
        {
            network::logger->error("UdpClient::Connect: invalid ip address({}): {}", ip_addr,
                                   ec.message());
            return false;
        }
        m_sock.connect(udp::endpoint(address, port_num), ec);
        if (ec)
        {
            network::logger->error("UdpClient::Connect: connect to {}:{} error: ", ip_addr,
                                   port_num, ec.message());
            return false;
        }

        m_is_connected = true;

        return true;
    }

    void Disconnect()
    {
        if (m_is_connected)
        {
            asio::error_code ec;
            m_sock.close(ec);
            if (ec)
            {
                network::logger->error("UdpClient::Disconnect: error closing socket: {}",
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
            network::logger->error("UdpClient::Send: error sending buffer: {}", ec.message());

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
            network::logger->error("UdpClient::Receive: error receiving buffer: {}", ec.message());

        return ret;
    }

    void SetRecvBufSize(uint32_t size)
    {
        asio::socket_base::receive_buffer_size option(size);
        m_sock.set_option(option);
    }

    void Shutdown()
    {
        asio::error_code ec;
        m_sock.shutdown(asio::socket_base::shutdown_both, ec);
        if (ec)
            network::logger->error("UdpClient::ShutDown: error stopping socket : {}", ec.message());
    }

    bool IsOpen() const { return m_is_connected; }

protected:
    udp::socket m_sock;
    std::atomic<bool> m_is_connected{false};
    std::mutex m_mutex;
};

}  // namespace cppbase
