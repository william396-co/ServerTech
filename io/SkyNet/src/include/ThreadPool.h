#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

using Task = std::function<void()>;

class ThreadPool {
   public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    template <typename F, typename... Args>
    auto add(F&& f, Args&&... args)
        -> std::future<typename std::result_of_t<F(Args...)>>;

   private:
    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop{};
};

template <typename F, typename... Args>
auto ThreadPool::add(F&& f, Args&&... args)
    -> std::future<typename std::result_of_t<F(Args...)>> {
    using return_type = typename std::result_of_t<F(Args...)>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock(tasks_mtx);
        if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]() { (*task)(); });
    }

    cv.notify_one();
    return res;
}
