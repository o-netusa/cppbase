/**************************************************************************
 * @file:  TcpServer.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include "Common.h"

namespace cppbase {

/**
 * @brief TCP Connection class
 */
class TcpConnection
{
public:
    static std::shared_ptr<TcpConnection> Create(asio::io_context& io_context)
    {
        struct make_shared_enabler : public TcpConnection
        {
            make_shared_enabler(asio::io_context& io_context) : TcpConnection(io_context) {}
        };
        return std::make_shared<make_shared_enabler>(io_context);
    }

    tcp::socket& GetSocket() { return m_socket; }

    uint32_t Send(const uint8_t* buffer, uint32_t bufsz)
    {
        uint32_t ret = 0;
        if (!buffer || bufsz == 0)
            return ret;

        asio::error_code error;
        ret = m_socket.write_some(asio::buffer(buffer, bufsz), error);
        if (error)
            network::logger->error("TcpConnection::Send: error sending data: {}", error.message());

        network::logger->debug("TcpConnection::Send: sent {} bytes data", ret);

        return ret;
    }

    uint32_t Receive(uint8_t* buffer, uint32_t bufsz)
    {
        uint32_t ret = 0;
        if (!buffer || bufsz == 0)
            return ret;

        asio::error_code error;
        ret = m_socket.read_some(asio::buffer(buffer, bufsz), error);
        if (error)
            network::logger->error("TcpConnection::Receive: error reading data: {}",
                                   error.message());

        network::logger->debug("TcpConnection::Receive: read {} bytes data", ret);

        return ret;
    }

private:
    TcpConnection(asio::io_context& io_context) : m_socket(io_context) {}
    tcp::socket m_socket;
};

/**
 * @brief TCP Server class
 */
class TcpServer
{
public:
    TcpServer(uint16_t port_num)
        : m_acceptor(network::io_context, tcp::endpoint(tcp::v4(), port_num))
    {}

    void Start(std::function<void(std::shared_ptr<TcpConnection>)> accept_handler)
    {
        if (m_started)
            return;

        m_accept_handler = accept_handler;
        StartAccept();
        m_started = true;
    }

    void Stop()
    {
        if (!m_started)
            return;

        if (m_started)
            m_acceptor.cancel();
        m_started = false;
    }

private:
    void StartAccept()
    {
        auto new_connection = TcpConnection::Create(network::io_context);
        m_acceptor.async_accept(new_connection->GetSocket(), [new_connection,
                                                              this](const asio::error_code& ec) {
            if (ec)
            {
                network::logger->error("TcpServer::StartAccept: accept handler error: {}",
                                       ec.message());
                return;
            }
            auto client_addr = new_connection->GetSocket().local_endpoint().address().to_string();
            network::logger->debug("TcpServer::StartAccept: accepted connection from: {}",
                                   client_addr);
            m_accept_handler(new_connection);
            StartAccept();
        });
    }

private:
    tcp::acceptor m_acceptor;
    std::function<void(std::shared_ptr<TcpConnection>)> m_accept_handler;
    bool m_started{false};
};
}  // namespace cppbase
