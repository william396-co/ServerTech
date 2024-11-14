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

#include "Macros.h"

using Task = std::function<void()>;

class ThreadPool {
   public:
    ThreadPool(size_t size = 10);
    ~ThreadPool();

    DISALLOW_COPY_AND_MOVE(ThreadPool);

    template <typename F, typename... Args>
    auto Add(F&& f, Args&&... args) -> std::future<typename std::result_of_t<F(Args...)>>;

   private:
    std::vector<std::thread> workers_;
    std::queue<Task> tasks_;
    std::mutex tasks_mtx_;
    std::condition_variable cv_;
    bool stop_{};
};

template <typename F, typename... Args>
auto ThreadPool::Add(F&& f, Args&&... args) -> std::future<typename std::result_of_t<F(Args...)>> {
    using return_type = typename std::result_of_t<F(Args...)>;

    auto task =
        std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock(tasks_mtx_);
        if (stop_) throw std::runtime_error("enqueue on stop_ped ThreadPool");

        tasks_.emplace([task]() { (*task)(); });
    }

    cv_.notify_one();
    return res;
}
