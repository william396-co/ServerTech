#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using Task = std::function<void()>;

class ThreadPool
{

public:
    ThreadPool( int size = 10 );
    ~ThreadPool();

    void add( Task const & task );

private:
    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop {};
};
