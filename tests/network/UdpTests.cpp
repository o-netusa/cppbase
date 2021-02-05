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
    UdpServer udp_server(1234);
}
