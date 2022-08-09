/**************************************************************************
 * @file:  TcpTests.cpp
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 **************************************************************************/

#include <gtest/gtest.h>
#include <network/TcpClient.h>
#include <network/TcpServer.h>

using namespace cppbase;

TEST(TCPTests, TCPServerClient)
{
    auto tcp_server = std::make_shared<TcpServer>("127.0.0.1", 1234);
    tcp_server->Start([](std::shared_ptr<TcpConnection> connection) {
        static char text[] = "hello";
        uint8_t buf[256] = {0};
        auto ret = connection->Receive(buf, 256);
        EXPECT_EQ(strlen(text), ret);
        EXPECT_STREQ(reinterpret_cast<char*>(buf), text);
        ret = connection->Send(buf, strlen(text));
        EXPECT_EQ(strlen(text), ret);
    });

    auto tcp_client = std::make_shared<TcpClient>();
    EXPECT_TRUE(tcp_client->Connect("127.0.0.1", 1234));
    std::string msg = "hello";
    auto ret = tcp_client->Send(reinterpret_cast<const uint8_t*>(msg.c_str()), msg.length());
    EXPECT_EQ(msg.length(), ret);
    uint8_t buf[256] = {0};
    ret = tcp_client->Receive(buf, 256);
    EXPECT_EQ(msg.length(), ret);
    EXPECT_STREQ(reinterpret_cast<char*>(buf), msg.c_str());
}
