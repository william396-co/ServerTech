#pragma once

#include <exception>
#include <stack>
#include <mutex>
#include <memory>

struct empty_stack : std::exception
{
    const char * what() const throw();
};

template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack() {}
    threadsafe_stack( threadsafe_stack const & other )
    {
        std::lock_guard lk( other.m );
        data = other.data;
    }
    threadsafe_stack & operator=( threadsafe_stack const & other ) = delete;

    void push( T new_val )
    {
        std::lock_guard lk( m );
        data.push( std::move( new_val ) );
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard lk( m );
        if ( data.empty() ) throw empty_stack();
        std::share_ptr<T> const res( std::make_shared<T>( std::move( data.top() ) ) );
        data.pop();
        return res;
    }
    void pop( T & value )
    {
        std::lock_guard lk( m );
        if ( data.empty() ) throw empty_stack();
        value = std::move( data.top() );
        data.pop();
    }

    bool empty() const
    {
        std::lock_guard lk( m );
        return data.empty();
    }

private:
    std::stack<T> data;
    mutable std::mutex m;
};
