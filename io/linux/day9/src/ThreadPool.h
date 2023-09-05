#pragma once

#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "util.h"

class ThreadPool
{
public:
    ThreadPool( int sz = 10 );
    ~ThreadPool();

    void add( Task task );

private:
    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
};
