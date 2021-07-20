/**************************************************************************
 * @file:  Semaphore.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

namespace cppbase {

class Semaphore
{
public:
    Semaphore(int32_t count = 0) : m_count(count) {}

    ~Semaphore() { Reset(); }

    void Notify()
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_count++;
        }
        // notify the waiting thread
        m_cv.notify_one();
    }
    void Wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_count == 0)
        {
            // wait on the mutex until notify is called
            m_cv.wait(lock);
        }
        m_count--;
    }
    bool TryWait(std::chrono::microseconds timeout /*us*/)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_count == 0)
        {
            m_cv.wait_for(lock, timeout);
            if (m_count == 0)
                return false;
        }
        m_count--;
        return true;
    }

    void Reset()
    {
        m_cv.notify_all();
        m_count = 0;
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    int32_t m_count;
};

}  // namespace cppbase
