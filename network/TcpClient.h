/**************************************************************************
 * @file:  TcpClient.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include "Common.h"

namespace cppbase {

class DLLEXPORT TcpClient
{
public:
    TcpClient() = default;
    ~TcpClient()
    {
        Disconnect();
    }

    bool Connect(const std::string& ip_addr, uint16_t port_num)
    {
        asio::error_code ec;
        if (m_is_connected)
        {
            logger->error("TcpClient::Connect: already connected.");
            return false;
        }
        auto address = asio::ip::address::from_string(ip_addr.c_str(), ec);
        if (ec)
        {
            logger->error("TcpClient::Connect: invalid ip address({}): {}", ip_addr, ec.message());
            return false;
        }
        m_sock.connect(tcp::endpoint(address, port_num), ec);
        if (ec)
        {
            logger->error("TcpClient::Connect: connect to {}:{} error: ", ip_addr, port_num, ec.message());
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
                logger->error("TcpClient::Disconnect: error closing socket: {}", ec.message());
            }
        }
        m_is_connected = false;
    }

    uint32_t Send(const uint8_t* buffer, uint32_t size)
    {
        uint32_t ret = 0;

        if (!buffer || size == 0 || !m_is_connected)
            return ret;

        asio::error_code ec;
        ret = m_sock.send(asio::buffer(buffer, size), 0, ec);
        if (ec)
            logger->error("TcpClient::Send: error sending buffer: {}", ec.message());

        return ret;
    }

    uint32_t Receive(uint8_t* buffer, uint32_t size)
    {
        uint32_t ret = 0;

        if (!buffer || size == 0 || !m_is_connected)
            return ret;

        asio::error_code ec;
        ret = m_sock.receive(asio::buffer(buffer, size), 0, ec);
        if (ec)
            logger->error("TcpClient::Receive: error receiving buffer: {}", ec.message());

        return ret;
    }

private:
    tcp::socket m_sock{GetIOContext()};
    bool m_is_connected{false};
};

}
