/**************************************************************************
 * @file:  UdpTests.cpp
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
**************************************************************************/

#include <gtest/gtest.h>

#include <network/UdpClient.h>
#include <network/UdpServer.h>

using namespace cppbase;

TEST(UDPTests, UDPServerClient)
{
    network::Init();

    auto udp_server = std::make_shared<UdpServer>(1234);
    udp_server->Start([udp_server](uint8_t* buffer, uint32_t bufsz, udp::endpoint& endpoint) {
        static char text[] = "hello";
        EXPECT_EQ(strlen(text), bufsz);
        EXPECT_STREQ(reinterpret_cast<char*>(buffer), text);
        auto ret = udp_server->Send(buffer, bufsz, endpoint);
        EXPECT_EQ(strlen(text), ret);
    });

    auto udp_client = std::make_shared<UdpClient>();
    EXPECT_TRUE(udp_client->Connect("127.0.0.1", 1234));
    std::string msg = "hello";
    auto ret = udp_client->Send(reinterpret_cast<const uint8_t*>(msg.c_str()), msg.length());
    EXPECT_EQ(msg.length(), ret);
    uint8_t buf[256] = {0};
    ret = udp_client->Receive(buf, 256);
    EXPECT_EQ(msg.length(), ret);
    EXPECT_STREQ(reinterpret_cast<char*>(buf), msg.c_str());

    network::Terminate();
}
