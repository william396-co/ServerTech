#include <iostream>
#include <coroutine>
#include <cstdint>

using namespace std;

struct resumable_thing
{
    struct promise_type
    {
        resumable_thing get_return_object()
        {
            return resumable_thing( coroutine_handle<promise_type>::from_promise( *this ) );
        }
        auto initial_suspend() { return suspend_never {}; }
        auto final_suspend() noexcept { return suspend_never {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    coroutine_handle<promise_type> _coroutine = nullptr;
    resumable_thing() = default;
    resumable_thing( resumable_thing const & ) = delete;
    resumable_thing & operator=( resumable_thing const & ) = delete;
    resumable_thing( resumable_thing && other ) noexcept
        : _coroutine { other._coroutine }
    {
        other._coroutine = nullptr;
    }
    resumable_thing & operator=( resumable_thing && other ) noexcept
    {
        if ( &other != this ) {
            _coroutine = other._coroutine;
            other._coroutine = nullptr;
        }
        return *this;
    }
    explicit resumable_thing( coroutine_handle<promise_type> coroutine )
        : _coroutine { coroutine }
    {
    }
    ~resumable_thing()
    {
        if ( _coroutine ) {
            _coroutine.destroy();
        }
    }

    void resume() { _coroutine.resume(); }
};

resumable_thing counter()
{
    cout << "counter: called:\n";
    uint32_t i = 0;
    while ( true ) {
        co_await std::suspend_always {};
        cout << "counter: resumed(#" << i++ << ")\n";
    }
}

int main()
{
    cout << "Main: calling counter\n";
    resumable_thing the_counter = counter();
    cout << "Main: resuming counter\n";
    the_counter.resume();
    the_counter.resume();
    the_counter.resume();
    the_counter.resume();
    the_counter.resume();
    the_counter.resume();
    cout << "Main done!\n";
    return 0;
}
