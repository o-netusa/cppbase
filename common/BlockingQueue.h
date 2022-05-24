/**************************************************************************
 * @file:  BlockingQueue.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

namespace cppbase {

/**
 * @brief BlockingQueue
 */
template <typename T>
class BlockingQueue
{
public:
    BlockingQueue(uint32_t max_size = 0)
        : m_max_size(max_size)
    {}
    ~BlockingQueue() = default;

    void Push(const T& data)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            while (m_max_size > 0 && m_queue.size() >= m_max_size)
            {
                m_queue.pop();
            }
            m_queue.push(data);
        }
        m_signal.notify_one();
    }

    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())
            m_signal.wait(lock);

        auto data = m_queue.front();
        m_queue.pop();
        return data;
    }

    T TryPop(int32_t timeout_ms = 0)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            if (m_signal.wait_for(lock, std::chrono::milliseconds(timeout_ms)) ==
                std::cv_status::timeout)
                throw std::runtime_error("BlockingQueue::TryPop timeout");

            if (m_queue.empty())
                throw std::runtime_error("BlockingQueue::TryPop empty");
        }
        auto value = m_queue.front();
        m_queue.pop();
        return value;
    }

    bool Empty() const { return m_queue.empty(); }

    void Clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::queue<T> empty;
        std::swap(m_queue, empty);
    }

    size_t Size() const { return m_queue.size(); }

private:
    uint32_t m_max_size{0};
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_signal;
};

}  // namespace cppbase
