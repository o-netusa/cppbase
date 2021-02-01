/*****************************************************************************
 * @file: ThreadPool_intl.h
 * @brief:
 *
 * Copyright 2021 O-Net Technologies (Group) Limited.
 ****************************************************************************/

#include "ThreadPool.h"
#include <logging/Logging.h>

namespace cppbase {

static auto logger = logging::GetLoggerForCurrentModule();

// #ifdef __linux__
#if 0
int32_t SetThreadPriority(ThreadPriority priority)
{
    pthread_t id = pthread_self();
    int policy;
    struct sched_param sp;
    int pri = static_cast<int>(priority);

    int rv = pthread_getschedparam(id, &policy, &sp);
    if (rv != 0)
        return rv;

    olog_info("ThreadUtils", "thread {}, policy: {}, priority: {}, new priority: {}",
              id, policy, sp.sched_priority, pri);

    if (sp.sched_priority == pri && policy == SCHED_RR)
        return 0;

    int priority_max = sched_get_priority_max(SCHED_RR);
    pri = std::min(pri, priority_max);
    sp.sched_priority = pri;
    rv = pthread_setschedparam(pthread_self(), SCHED_RR, &sp);
    if (rv != 0)
    {
        olog_warning("ThreadUtils", "warning: sched_rr set fail: {}", strerror(errno));
        return rv;
    }
    else
    {
        pthread_getschedparam(pthread_self(), &policy, &sp);
        olog_info("ThreadUtils", "thread {}, new policy: {}, priority is: {}", id, policy,
                  sp.sched_priority);
    }

    return 0;
}

int32_t GetThreadPriority(int64_t id)
{
    int policy;
    struct sched_param sp;
    int rv = pthread_getschedparam(id, &policy, &sp);
    if (rv != 0)
        throw InternalErrorException("Error getting thread sched param");
    return sp.sched_priority;
}

#else

int32_t SetThreadPriority(ThreadPriority)
{
    return 0;
}

int32_t GetThreadPriority(int64_t)
{
    return 0;
}

#endif

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(uint32_t threads, ThreadPriority priority, uint32_t cpu_reserved)
    : m_priority(priority),
      m_cpu_reserved(cpu_reserved)
{
    static uint32_t cpu_nums = std::thread::hardware_concurrency();
    auto set_affinity = [&](uint32_t index, std::thread& thread) {
        if (cpu_reserved && cpu_reserved < cpu_nums)
        {
#ifdef __linux__
            uint32_t cpu_id = index % (cpu_nums - cpu_reserved);
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(cpu_id, &cpuset);
            auto rc = pthread_setaffinity_np(thread.native_handle(),
                                             sizeof(cpu_set_t), &cpuset);
            if (rc != 0)
            {
                logger->error("Error calling pthread_setaffinity_np: {}", rc);
            }
#endif
        }
    };

    for(uint32_t i = 0; i < threads; ++i)
    {
        m_threads.emplace_back([this, priority] {
            SetThreadPriority(priority);
            for(;;)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_queue_mutex);
                    m_condition.wait(lock, [this]{ return m_stop || !m_tasks.empty(); });
                    if (m_stop && m_tasks.empty())
                        return;
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                task();
            }
        });
        set_affinity(i, m_threads[i]);
    }
    logger->info("Created thread pool with {} threads at priority {}, {} CPU core(s) reserved",
        threads, priority, cpu_reserved);
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_stop = true;
    }
    m_condition.notify_all();
    for(std::thread &thread: m_threads)
        thread.join();
}

ThreadPriority ThreadPool::GetThreadPriority() const
{
    return m_priority;
}

uint32_t ThreadPool::GetReservedCpu() const
{
    return m_cpu_reserved;
}

} // namespace cppbase
