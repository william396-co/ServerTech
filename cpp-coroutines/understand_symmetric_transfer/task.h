#pragma once

#include <coroutine>
#include <utility>
#include <exception>

class task
{
public:
    class promise_type
    {};

    task( task && t ) noexcept
        : coro_( std::exchange( t.coro_, {} ) ) {}

    ~task()
    {
        if ( coro_ ) {
            coro_.destroy();
        }
    }

    class awaiter
    {};
    awaiter operator co_await() && noexcept;

private:
    explicit task( std::coroutine_handle<promise_type> h ) noexcept
        : coro_ { h } {}

    std::coroutine_handle<promise_type> coro_;
};

class task::promise_type
{
public:
    task get_return_object() noexcept
    {
        return task { std::coroutine_handle<promise_type>::from_promise( *this ) };
    }

    std::suspend_always initial_suspend() noexcept { return {}; }
    void return_void() noexcept {}
    void unhandled_exception() noexcept
    {
        std::terminate();
    }

    struct final_awaiter
    {
        bool await_ready() noexcept { return false; }

        void await_suspend( std::coroutine_handle<promise_type> h ) noexcept
        {
            h.promise.continuation.resume();
        }
    };
    final_awaiter final_suspend() noexcept
    {
        return {};
    }
    std::coroutine_handle<> continuation;
};
