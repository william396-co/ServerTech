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
    // 内置线程工作类
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
                // 为线程环境锁加锁，互访问工作线程的休眠和唤醒
                std::unique_lock<std::mutex> lock(m_pool->m_condition_mutex); // 获取锁

                // 如果任务队列为空，阻塞当前线程
                // if (m_pool->m_queue.empty())
                //{
                //      //cout << " pool is empty" << endl;
                //      m_pool->m_condition_lock.wait(lock);////等待条件变量通知，开启线程
                // }

                // 等待条件满足(如果不满足)
                m_pool->m_condition_lock.wait(lock, [&]() { return !m_pool->m_queue.empty(); }); // wait(lock,pred) = (if(!pred()) wait();

                dequeued = m_pool->m_queue.dequeue(func);                                        // 消费者
                lock.unlock();

                // 如果成功取出，执行工作函数
                if (dequeued)
                    func();
            }
        }

    private:
        int m_id;
        ThreadPool* m_pool; // 所属线程池
    };

private:
    bool m_shutdown = false; // 线程池是否关闭
    SafeQueue<std::function<void()>> m_queue;

    std::vector<std::thread> m_threads;
    std::mutex m_condition_mutex;             // 线程休眠锁互斥量
    std::condition_variable m_condition_lock; // 线程环境锁
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
        m_condition_lock.notify_all(); // 通知 唤醒所有工作线程

        for (size_t i = 0; i < m_threads.size(); ++i)
            if (m_threads[i].joinable()) // 判断线程是否在等待(可加入)
                m_threads[i].join();       // 则加入等待队列
    }

    // submit a function to be executed asynchronously by the pool
    // 线程的主要工作函数，使用后置返回类型，自动判断函数返回值
    template<typename F, typename... Args>
    auto submit(F&& f, Args &&... args) -> std::future<decltype(f(args...))> // 推导返回类型
    {

        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // 连接函数和参数定义，特殊函数类型, 避免左、右值错误

        // Encapsulate it into a shared ptr in order to be able to copy construct/assign
        // 封装获取任务对象，方便另外一个线程查看结果

        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        // wrap packaged task into void function
        std::function<void()> wrapper_func = [task_ptr]() {
            (*task_ptr)();
		};

        // 队列通过安全封装函数，并压入安全队列
        unique_lock<mutex> lock(m_condition_mutex); // 生产者
        m_queue.enqueue(wrapper_func);
        lock.unlock();

        // 唤醒一个等待中的线程
        m_condition_lock.notify_one();

        // 返回先前注册的任务指针
        return task_ptr->get_future();
    }
};