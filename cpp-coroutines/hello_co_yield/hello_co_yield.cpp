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
            std::cout << value_ << "\n";
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
    co_yield s;
    std::cout << "world!\n";
}

int main()
{
    HelloCoroutine co = hello();

    std::cout << "calling resume\n";
    co.handle.resume();

    std::cout << "destroy\n";
    co.handle.destroy();

    return 0;
}
