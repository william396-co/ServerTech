#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>
#include <memory>

template<typename T>
class threadsafe_queue
{
public:
    threadsafe_queue() {}
    void push( T new_val )
    {
        std::lock_guard lk( m );
        data.push( std::move( new_val ) );
        cond.notify_one();
    }

    void wait_and_pop( T & value )
    {
        std::unique_lock lk( m );
        cond.wait( lk, [this] { return !data.empty(); } );
        value = std::move( data.front() );
        data.pop();
    }

    std::share_ptr<T> wait_and_pop()
    {
        std::unique_lock lk( m );
        cond.wait( lk, [this] { return !data.empty(); } );
        std::shared_ptr<T> res( std::make_shared<T>( std::move( data.front() ) ) );
        data.pop();
        return res;
    }
    bool try_pop( T & value )
    {
        std::lock_guard lk( m );
        if ( data.empty() )
            return false;
        value = std::move( data.front() );
        data.pop();
        return true;
    }

    std::share_ptr<T> try_pop()
    {
        std::lock_guard lk( m );
        if ( data.empty() )
            return std:: : shared_ptr<T>();
        std::shared_ptr<T> res( std::make_shared<T>( std::move( data.front() ) ) );
        data.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard lk( m );
        return data.empty();
    }

private:
    mutable std::mutex m;
    std::queue<T> data;
    std::condition_variable cond;
};
