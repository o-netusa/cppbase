/*****************************************************************************
 * @file: Timer.h
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
 ****************************************************************************/

#pragma once

#include <chrono>

namespace cppbase {

using ns = std::chrono::nanoseconds;
using us = std::chrono::microseconds;
using ms = std::chrono::milliseconds;
using s = std::chrono::seconds;
using m = std::chrono::minutes;
using h = std::chrono::hours;

template <typename DurationType> class Timer;
using TimerNs = Timer<ns>;
using TimerUs = Timer<us>;
using TimerMs = Timer<ms>;
using TimerS  = Timer<s>;
using TimerM  = Timer<m>;
using TimerH  = Timer<h>;

template <typename DurationType = us>
class Timer
{
public:
    Timer() : m_started(true), m_last(std::chrono::steady_clock::now())
    {}

    void Start()
    {
        m_started = true;
        m_last = std::chrono::steady_clock::now();
    }
    void Stop()
    {
        m_started = false;
    }

    bool IsRunning() const
    {
        return m_started;
    }
    typename DurationType::rep Elapsed() const
    {
        if (!m_started)
            return 0;

        auto elapsed = std::chrono::duration_cast<DurationType>(std::chrono::steady_clock::now() - m_last);
        return elapsed.count();
    }
    
private:
    bool m_started;
    std::chrono::steady_clock::time_point m_last;
};

} // namespace cppbase
