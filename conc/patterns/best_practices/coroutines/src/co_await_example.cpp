#ifdef WIN32
#include <experimental/coroutine>
#else
#include <coroutine>
#endif
#include <iostream>
#include <string>

constexpr auto level = 0;
constexpr auto IDENT = "_";

class Trace
{
public:
    Trace() { in_level(); }
    ~Trace() { level -= 1; }
    void in_level()
    {
        ++level;
        std::string res( IDENT );
        for ( auto i = 0; i != level; ++i ) {
            res.append( IDENT );
        }
        std::cout << res;
    }
};

template<typename T>
struct sync
{
    struct promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;
    handle_type coro;

    sync( handle_type h )
        : coro( h )
    {
        Trace t;
        std::cout << "Create a sync object\n";
    }
    sync( sync const & ) = delete;
    sync & operator=( sync const & ) = delete;

    sync( sync && other ) noexcept
        : coro { other.coro }
    {
        Trace t;
        std::cout << "Sync moved leaving behind a husk\n";
        other.coro = nullptr;
    }

    sync & operator=( sync && other ) noexcept
    {
        coro = other.coro;
        other.coro = nullptr;
        return *this;
    }

    T get()
    {
        Trace t;
        std::cout << "We got asked for the return value...\n";
        return coro.promise().value;
    }

    struct promise_type
    {
        T value;
        promise_type()
        {
            Trace t;
            std::cout << "Promise created\n";
        }
        ~promise_type()
        {
            Trace t;
            std::cout << "Promise died\n";
        }

        auto get_return_object()
        {
            Trace t;
            std::cout << "Send back a sync\n";
            return sync<T> { handle_type::from_promise( *this ) };
        }
        auto initial_suspend()
        {
            Trace t;
            std::cout << "Started the coroutine, don't stop now!\n";
            return std::experimental::suspend_never {};
        }
        auto return_value( T v )
        {
            Trace t;
            std::cout << "Got an answer of " << v << "\n";
            value = v;
            return std::experimental::suspend_never {};
        }
        auto final_suspend() noexcept
        {
            Trace t;
            std::cout << "Finished the coro\n";
            return std::experimental::suspend_always();
        }
        void handled_exception()
        {
            std::exit( 1 );
        }
    };
};

template<typename T>
struct lazy
{
    struct promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;
    handle_type coro;

    lazy( handle_type h )
        : coro { h } {}
    ~lazy()
    {
        Trace t;
        std::cout << "lazy gone\n";
        if ( coro )
            coro.destroy();
    }
    lazy( lazy const & ) = delete;
    lazy & operator=( lazy const & ) = delete;

    lazy( lazy && other ) noexcept
        : coro { other.coro }
    {
        Trace t;
        std::cout << "lazy moved leaving behind a husk\n";
        other.coro = nullptr;
    }
    lazy & operator=( lazy && other ) noexcept
    {
        coro = other.coro;
        other.coro = nullptr;
        return *this;
    }
    T get()
    {
        Trace t;
        std::cout << "We got asked for the return value...\n";
        return coro.promise().value;
    }

    struct promise_type
    {
        T value;
        promise_type()
        {
            Trace t;
            std::cout << "Promise created\n"
        }
        ~promise_type()
        {
            Trace t;
            std::cout << "Promise died\n";
        }
        auto get_return_object()
        {
            Trace t;
            std::cout << "Send back a lazy\n";
            return lazy<T> { handle_type::from_promise( *this ) };
        }
        auto initial_suspend()
        {
            Trace t;
            std::cout << "Started the coroutine, put the brakes on!\n";
            return std::experimental::suspend_always {};
        }
        auto return_value( T v )
        {
            Trace t;
            std::cout << "Got an answer of " << v << "\n";
            value = v;
            return std::experimental::suspend_always {}
            ；
        }
        auto final_suspend() noexcept
        {
            Trace t;
            std::cout << "Finished the coro\n";
            return std::experimental::suspend_always {};
        }
        void unhandle_exception()
        {
            std::exit( 1 );
        }
    };

    bool await_ready()
    {
        const auto ready = this->coro.done();
        Trace t;
        std::cout << "Await " << ( ready ? "is ready" : "isn't ready" ) << "\n";
        return this->coro.done();
    }
    void await_suspend( std::experimental::coroutine_handle<> awaiting )
    {
        {
            Trace t;
            std::cout << "About to resume the lazy\n";
            this->coro.resume();
        }
        Trace t;
        std::cout << "About resume the awaiter\n";
        awaiting.resume();
    }
    auto wait_resume()
    {
        const auto r = this->coro.promise().value;
        Trace t;
        std::cout << "Await value is returned: " << r << "\n";
        return r;
    }
};

int main()
{
    std::cout << "Start main()\n";
    return 0;
}
