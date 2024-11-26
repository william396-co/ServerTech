#include <coroutine>
#include <iostream>
#include <string_view>

/*
 * co_yield<expression>  equal to co_yield promise.yield_value(<expression>)
 */

struct HelloCoroutine
{
    struct HelloPromise
    {
        std::string_view value_;

        HelloCoroutine get_return_object()
        {
            return std::coroutine_handle<HelloPromise>::from_promise( *this );
        }
        std::suspend_never initial_suspend() { return {}; }
        // 在final_suspend()挂起协程，所以要手动destroy
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}

        std::suspend_always yield_value( std::string_view value )
        {
            value_ = value;
            std::cout << "yeild " << value_ << "\n";
            return {};
        }
    };

    using promise_type = HelloPromise;
    HelloCoroutine( std::coroutine_handle<promise_type> h )
        : handle( h ) {}

    std::coroutine_handle<promise_type> handle;
};

HelloCoroutine hello()
{
    std::string_view s = "Hello";
    co_yield s; // equal to co_await promise.yield_value(<expression>)
    std::cout << "world!\n";
}

int main()
{
    {
        HelloCoroutine co = hello();

        std::cout << "calling resume\n";
        co.handle.resume();

        if ( !co.handle.done() ) {
            std::cout << " resuming again\n";
            co.handle.resume(); // undefined behavior to resume() a coroutine that is suspend at the final_suspend point
        }

        if ( co.handle.done() ) {
            std::cout << "destroy\n";
            co.handle.destroy();
        }
    }
    {
        std::cout << "=======================================\n";
        auto co = hello();
        std::cout << "calling resume\n";
        co.handle.resume();

        // coroutine_handle::done() means coroutine_handle at final suspend-point, can't resume(), only destroy(), the resume() function
        // pointer as an nullptr
        if ( co.handle.done() ) {
            std::cout << "co is done, then destroy\n";
            co.handle.destroy();
        } else {
            std::cout << "resuming again\n";
            co.handle.resume();
        }
    }

    return 0;
}
