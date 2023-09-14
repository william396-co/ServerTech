#pragma once

#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>
#include <type_traits>
#include <stdexcept>
#include "util.h"
#include "Macros.h"

class ThreadPool
{
public:
    explicit ThreadPool( int sz = 10 );
    ~ThreadPool();

    DISALLOW_COPY_AND_MOVE( ThreadPool );

    //    void add( Task task );

    template<typename F, typename... Args>
    auto add( F && fn, Args &&... args ) -> std::future<typename std::result_of_t<F( Args... )>>;

private:
    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
};

template<typename F, typename... Args>
auto ThreadPool::add( F && fn, Args &&... args ) -> std::future<typename std::result_of_t<F( Args... )>>
{
    using return_type = typename std::result_of_t<F( Args... )>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind( std::forward<F>( fn ), std::forward<Args>( args )... ) );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock( tasks_mtx );
        // don't allow eqeueing after stoping the pool
        if ( stop )
            throw std::runtime_error( "enqueue on stopped ThreadPool" );
        tasks.emplace( [task]() { ( *task )(); } );
    }
    cv.notify_one();
    return res;
}
