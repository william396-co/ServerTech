#include <coroutine>
#include <iostream>
#include <cstdint>
#include <exception>

template<typename T>
struct Generator
{
    // The class name 'Generator' is our choice and it is not required for coroutine
    // magic. Compiler recognizes coroutine by the presence of 'co_yield' keyword.
    // You can use name 'MyGenerator' (or any other name) instead as long as you include
    // nested struct promise_type with 'MyGenerator get_return_object()' method.

    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    struct promise_type
    {
        T value_;
        std::exception_ptr exception_;
        Generator get_return_object()
        {
            return Generator( handle_type::from_promise( *this ) );
        }
        auto initial_suspend()
        {
            return std::suspend_always {};
        }
        auto final_suspend() noexcept
        {
            return std::suspend_always {};
        }
        void return_void() {}

        template<std::convertible_to<T> From> // C++20 concept
        std::suspend_always yield_value( From && from )
        {
            value_ = std::forward<From>( from ); // caching the result in promise
            return {};
        }
        // saving exception
        void unhandled_exception() { exception_ = std::current_exception(); }
    };

    explicit Generator( handle_type h )
        : h_ { h }
    {
    }
    ~Generator()
    {
        if ( h_ ) {
            h_.destroy();
        }
    }
    explicit operator bool()
    {
        fill();
        return !h_.done();
    }
    T operator()()
    {
        fill();
        full_ = false;
        return std::move( h_.promise().value_ );
    }

private:
    bool full_ { false };
    void fill()
    {
        if ( !full_ ) {
            h_();
            if ( h_.promise().exception_ )
                std::rethrow_exception( h_.promise().exception_ );
            full_ = true;
        }
    }
    handle_type h_;
};

Generator<uint64_t>
fibonacci_seq( uint32_t n )
{
    if ( 0 == n )
        co_return;
    if ( n > 94 )
        throw std::runtime_error( "Too big Fibonacci sequence,Elements would overflow." );

    co_yield 0;

    if ( n == 1 )
        co_return;
    co_yield 1;
    if ( 2 == n )
        co_return;

    uint64_t a = 0, b = 1;
    for ( auto i = 2; i < n; ++i ) {
        uint64_t next = a + b;
        co_yield next;
        a = b;
        b = next;
    }
}

int main()
{

    try {
        auto gen = fibonacci_seq( 90 );
        for ( auto i = 0; gen; ++i ) {
            std::cout << "fib(" << i << ")=" << gen() << "\n";
        }
    } catch ( std::exception const & ex ) {
        std::cerr << "Exception:" << ex.what() << "\n";
    } catch ( ... ) {
        std::cerr << "Unkown exception.\n";
    }

    return 0;
}
