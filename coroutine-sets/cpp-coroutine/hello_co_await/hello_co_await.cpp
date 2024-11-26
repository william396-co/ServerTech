#include <coroutine>
#include <iostream>

struct HelloCoroutine
{

    struct HelloPromise
    {
        HelloCoroutine get_return_object()
        {
            return std::coroutine_handle<HelloPromise>::from_promise( *this );
        }
        std::suspend_never initial_suspend() { return {}; }
        // 在final_suspend()挂起协程，所以要手动destroy
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
    };

    using promise_type = HelloPromise;
    HelloCoroutine( std::coroutine_handle<promise_type> h )
        : handle( h ) {}

    std::coroutine_handle<promise_type> handle;
};

HelloCoroutine hello()
{
    std::cout << "hello \n";
    co_await std::suspend_always {};
    std::cout << "world!\n";
}

int main()
{
    HelloCoroutine co = hello();

    std::cout << "calling resume\n";
    co.handle.resume();

    if ( co.handle.done() ) {
        std::cout << "destroy\n";
        co.handle.destroy();
    }

    return 0;
}
