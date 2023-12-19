#pragma once

#include <queue>
#include <mutex>

template<typename T>
class SafeQueue
{
public:
    SafeQueue() {}
    ~SafeQueue() {}

    bool empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }
    size_t size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }
    void enqueue(const T& t)
    {
		std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(t);
    }

    bool dequeue(T& t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_queue.empty())
            return false;
        t = std::move(m_queue.front()); // get first
        m_queue.pop();

        return true;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};