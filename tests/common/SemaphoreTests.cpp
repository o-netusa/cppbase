/**************************************************************************
 * @file:  SemaphoreTests.cpp
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 **************************************************************************/

#include <common/Semaphore.h>
#include <gtest/gtest.h>

#include <thread>

TEST(SemaphoreTests, ProducerAndConsumer)
{
    cppbase::Semaphore sem(1);

    std::thread producer([&]() {
        for (uint32_t i = 0; i < 10; ++i)
        {
            sem.Notify();
            std::cout << "Producer: " << i << std::endl;
        }
    });

    std::thread consumer([&]() {
        for (uint32_t i = 0; i < 10; ++i)
        {
            sem.Wait();
            std::cout << "Consumer: " << i << std::endl;
        }
    });

    producer.join();
    consumer.join();
}
