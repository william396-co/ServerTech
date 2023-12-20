#pragma once
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
public:
    void push( T new_val )
    {
        std::lock_guard lock( mut );
        data_queue.push( new_val );
        data_cond.notify_one();
    }
    void wait_and_pop( T & value )
    {
        std::unique_lock lock( mut );
        //  acquired the lock and check lambda ,if condition hasn't satisfied,
        //  unlock and resume waiting if condition has satisfied,returning from wait() with mutex still locked
        data_cond.wait( lock, [this] { return !data_queue.empty(); } );
        value = data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock lock( mut );
        data_cond.wait( lock, [this] { return !data_queue.empty(); } );
        std::shared_ptr<T> res( std::make_shared<T>( data_queue.front() ) );
        data_queue.pop();
        return res;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard lock( mut );
        if ( data_queue.empty() )
            return std::make_shared<T>();
        std::shared_ptr<T> res( std::make_shared<T>( data_queue.front() ) );
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard lock( mut );
        return data_queue.empty();
    }

private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
};

