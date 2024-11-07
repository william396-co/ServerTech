#include "ThreadPool.h"

#include <algorithm>
#include <stdexcept>

ThreadPool::ThreadPool(int size) {
  for (auto i = 0; i != size; ++i) {
    threads.emplace_back(std::thread([this]() {
      while (true) {
        Task task{};
        {
          std::unique_lock lock(tasks_mtx);
          cv.wait(lock, [this]() { return stop || !tasks.empty(); });
          if (stop && tasks.empty()) return;
          task = std::move(tasks.front());
          tasks.pop();
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock lock(tasks_mtx);
    stop = true;
  }
  cv.notify_one();
  std::for_each(threads.begin(), threads.end(), [](auto&& t) {
    if (t.joinable()) t.join();
  });
}

/*
void ThreadPool::add( Task const & task )
{
    {
        std::unique_lock lock( tasks_mtx );
        if ( stop )
            throw std::runtime_error( "ThreadPool already stop, can't add task
any more!" ); tasks.emplace( task );
    }
    cv.notify_one();
}*/
