/**************************************************************************
 * @file:  BlockingQueue.h
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
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
    BlockingQueue() = default;
    ~BlockingQueue() = default;

    void Push(const T& data)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(data);
        }
        m_signal.notify_one();
    }

    void Pop(T& data)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())
            m_signal.wait(lock);

        data = m_queue.front();
        m_queue.pop();
    }

    void Pop(T& data, const std::function<bool()>& f)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty() && f())
            m_signal.wait(lock);

        if (!f())
            return;
        data = m_queue.front();
        m_queue.pop();
    }

    void PopFuncNotify()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_signal.notify_one();
    }

    bool TryPop(T& value, int32_t timeout_ms = 0)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            if (m_signal.wait_for(lock, std::chrono::milliseconds(timeout_ms)) ==
                std::cv_status::timeout)
                return false;

            if (m_queue.empty())
                return false;
        }
        value = m_queue.front();
        m_queue.pop();
        return true;
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
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_signal;
};

}  // namespace cppbase
