#pragma once

#include <coroutine>
#include <utility>
#include <exception>
#include <iostream>

class task
{
public:
    class promise_type
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

            std::coroutine_handle<> await_suspend( std::coroutine_handle<promise_type> h ) noexcept
            {
                return h.promise().continuation;
            }
            void await_resume() noexcept {}
        };
        final_awaiter final_suspend() noexcept
        {
            return {};
        }
        std::coroutine_handle<> continuation;
    };

    task( task && t ) noexcept
        : coro_( std::exchange( t.coro_, {} ) ) {}

    ~task()
    {
        if ( coro_ ) {
            coro_.destroy();
        }
    }

    class awaiter
    {
    public:
        bool await_ready() noexcept
        {
            return false;
        }

        void await_suspend( std::coroutine_handle<> continuation ) noexcept
        {
            // Store the continuation in the task's promise so that the final_suspend()
            // knows to resume the coroutine when the task completes.
            coro_.promise().continuation = continuation;
            // Then we resume the task's coroutine, which is currently suspended
            // at the initial-suspend-point(ie. at the open curly brace)
            coro_.resume();
        }

        void await_resume() noexcept {}

    private:
        friend task;
        explicit awaiter( std::coroutine_handle<promise_type> h ) noexcept
            : coro_ { h } {}

        std::coroutine_handle<promise_type> coro_;
    };

    awaiter operator co_await() && noexcept
    {
        return awaiter { coro_ };
    }

private:
    explicit task( std::coroutine_handle<promise_type> h ) noexcept
        : coro_ { h } {}

    std::coroutine_handle<promise_type> coro_;
};

struct sync_wait_task
{
    struct promise_type
    {
        sync_wait_task get_return_object() noexcept
        {
            return sync_wait_task { std::coroutine_handle<promise_type>::from_promise( *this ) };
        }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() noexcept { std::terminate(); }
    };

    std::coroutine_handle<promise_type> coro_;
    explicit sync_wait_task( std::coroutine_handle<promise_type> h ) noexcept
        : coro_ { h } {}

    sync_wait_task( sync_wait_task && t ) noexcept
        : coro_ { t.coro_ }
    {
        t.coro_ = {};
    }

    ~sync_wait_task()
    {
        if ( coro_ ) {
            coro_.destroy();
        }
    }
    static sync_wait_task start( task && t )
    {
        co_await std::move( t );
    }

    bool done()
    {
        return coro_.done();
    }
};

struct manual_executor
{
    struct schedule_op
    {
        manual_executor & executor_;
        schedule_op * next_ = nullptr;
        std::coroutine_handle<> continuation_;

        schedule_op( manual_executor & executor )
            : executor_ { executor } {}

        bool await_ready() noexcept { return false; }
        void await_suspend( std::coroutine_handle<> continuation ) noexcept
        {
            continuation_ = continuation;
            next_ = executor_.head_;
            executor_.head_ = this;
        }
        void await_resume() noexcept {}
    };

    schedule_op * head_ {};

    schedule_op schedule() noexcept
    {
        return schedule_op { *this };
    }

    void drain()
    {
        while ( head_ ) {
            auto * item = head_;
            head_ = item->next_;
            item->continuation_.resume();
        }
    }
    void sync_wait( task && t )
    {
        auto t2 = sync_wait_task::start( std::move( t ) );
        while ( !t2.done() ) {
            drain();
        }
    }
};

task completes_synchronously()
{
    std::cout << __FUNCTION__ << "\n";
    co_return;
}

task loop_synchronously( int count )
{
    std::cout << __FUNCTION__ << "(" << count << ")\n";
    for ( int i = 0; i != count; ++i ) {
        co_await completes_synchronously();
    }
    std::cout << __FUNCTION__ << "(" << count << ")returing\n";
}
