#include "ThreadPool.h"

#include <algorithm>
#include <stdexcept>

ThreadPool::ThreadPool(size_t size) {
    for (size_t i = 0; i != size; ++i) {
        workers_.emplace_back(std::thread([this]() {
            while (true) {
                Task task{};
                {
                    std::unique_lock lock(tasks_mtx_);
                    cv_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) return;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock(tasks_mtx_);
        stop_ = true;
    }
    cv_.notify_one();
    std::for_each(workers_.begin(), workers_.end(), [](auto&& t) {
        // if (t.joinable()) t.join();
        t.detach();
    });
}
