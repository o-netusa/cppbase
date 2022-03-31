/**************************************************************************
 * @file:  TcpServer.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <atomic>

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

        std::lock_guard<std::mutex> lock(m_mutex);

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

        std::lock_guard<std::mutex> lock(m_mutex);

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
    std::mutex m_mutex;
};

/**
 * @brief TCP Server class
 */
class TcpServer
{
public:
    TcpServer(const std::string& ip, uint16_t port_num)
        : m_acceptor(network::context.io_context, tcp::endpoint(address::from_string(ip), port_num))
    {
        m_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    }

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

        m_started = false;
        if (m_acceptor.is_open())
        {
            m_acceptor.cancel();
            m_acceptor.close();
        }
    }

    bool IsOpen() { return m_started; }

protected:
    void StartAccept()
    {
        auto new_connection = TcpConnection::Create(network::context.io_context);
        m_acceptor.async_accept(new_connection->GetSocket(), [new_connection,
                                                              this](const asio::error_code& ec) {
            if (ec)
            {
                network::logger->error("TcpServer::StartAccept: accept handler error: {}",
                                       ec.message());
                return;
            }
            if (!m_started)
                return;
            auto client_addr = new_connection->GetSocket().local_endpoint().address().to_string();
            network::logger->debug("TcpServer::StartAccept: accepted connection from: {}",
                                   client_addr);
            if (m_accept_handler)
                m_accept_handler(new_connection);
            if (m_started)
                StartAccept();
        });
    }

protected:
    tcp::acceptor m_acceptor;
    std::function<void(std::shared_ptr<TcpConnection>)> m_accept_handler;
    std::atomic<bool> m_started{false};
};
}  // namespace cppbase
