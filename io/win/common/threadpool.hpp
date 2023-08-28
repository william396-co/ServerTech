#pragma once
#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <queue>
#include <vector>

#include "safeQueue.hpp"
#include "singleton.hpp"

class ThreadPool : public Singleton<ThreadPool>
{
private:
    // �����̹߳�����
    class ThreadWork
    {
    public:
        ThreadWork(ThreadPool* pool, int id)
            : m_id{ id }, m_pool(pool) {}

        void operator()()
        {
            std::function<void()> func;

            bool dequeued = false;

            while (!m_pool->m_shutdown) {
                // Ϊ�̻߳����������������ʹ����̵߳����ߺͻ���
                std::unique_lock<std::mutex> lock(m_pool->m_condition_mutex); // ��ȡ��

                // ����������Ϊ�գ�������ǰ�߳�
                // if (m_pool->m_queue.empty())
                //{
                //      //cout << " pool is empty" << endl;
                //      m_pool->m_condition_lock.wait(lock);////�ȴ���������֪ͨ�������߳�
                // }

                // �ȴ���������(���������)
                m_pool->m_condition_lock.wait(lock, [&]() { return !m_pool->m_queue.empty(); }); // wait(lock,pred) = (if(!pred()) wait();

                dequeued = m_pool->m_queue.dequeue(func);                                        // ������
                lock.unlock();

                // ����ɹ�ȡ����ִ�й�������
                if (dequeued)
                    func();
            }
        }

    private:
        int m_id;
        ThreadPool* m_pool; // �����̳߳�
    };

private:
    bool m_shutdown = false; // �̳߳��Ƿ�ر�
    SafeQueue<std::function<void()>> m_queue;

    std::vector<std::thread> m_threads;
    std::mutex m_condition_mutex;             // �߳�������������
    std::condition_variable m_condition_lock; // �̻߳�����
public:
    ThreadPool(size_t n_threads)
        : m_shutdown(false), m_threads(std::vector<std::thread>(n_threads)) {}

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;

    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    void init()
    {
        for (size_t i = 0; i != m_threads.size(); ++i)
            m_threads[i] = std::thread(ThreadWork(this, i));
    }

    void shutdown()
    {
        // cout << "shutdown" << endl;
        m_shutdown = true;
        m_condition_lock.notify_all(); // ֪ͨ �������й����߳�

        for (size_t i = 0; i < m_threads.size(); ++i)
            if (m_threads[i].joinable()) // �ж��߳��Ƿ��ڵȴ�(�ɼ���)
                m_threads[i].join();       // �����ȴ�����
    }

    // submit a function to be executed asynchronously by the pool
    // �̵߳���Ҫ����������ʹ�ú��÷������ͣ��Զ��жϺ�������ֵ
    template<typename F, typename... Args>
    auto submit(F&& f, Args &&... args) -> std::future<decltype(f(args...))> // �Ƶ���������
    {

        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // ���Ӻ����Ͳ������壬���⺯������, ��������ֵ����

        // Encapsulate it into a shared ptr in order to be able to copy construct/assign
        // ��װ��ȡ������󣬷�������һ���̲߳鿴���

        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        // wrap packaged task into void function
        std::function<void()> wrapper_func = [task_ptr]() {
            (*task_ptr)();
		};

        // ����ͨ����ȫ��װ��������ѹ�밲ȫ����
        unique_lock<mutex> lock(m_condition_mutex); // ������
        m_queue.enqueue(wrapper_func);
        lock.unlock();

        // ����һ���ȴ��е��߳�
        m_condition_lock.notify_one();

        // ������ǰע�������ָ��
        return task_ptr->get_future();
    }
};