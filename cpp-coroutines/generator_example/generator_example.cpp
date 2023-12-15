#include <coroutine>
#include <numeric>
#include <exception>

template<typename T>
struct generator
{
    struct promise_type
    {
        T current_value;
        auto initial_suspend() { return std::suspend_always(); }
        auto final_suspend() noexcept { return std::suspend_always(); }
        generator get_return_object() { return generator { this }; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
        std::suspend_always yield_value( T value )
        {
            current_value = value;
            return {};
        }
    };

    struct iterator
    {
        std::coroutine_handle<promise_type> _Coro;
        bool _Done;

        iterator( std::coroutine_handle<promise_type> Coro, bool Done )
            : _Coro { Coro }, _Done { Done }
        {
        }

        iterator & operator++()
        {
            _Coro.resume();
            _Done = _Coro.done();
            return *this;
        }
        bool operator==( iterator const & other ) const
        {
            return _Done == other._Done;
        }
        bool operator!=( iterator const & other ) const
        {
            return !( *this == other );
        }
        T const & operator*() const
        {
            return _Coro.promise().current_value;
        }
        T const * operator->() const
        {
            return &( operator*() );
        }
    };

    iterator begin()
    {
        p.resume();
        return { p, p.done() };
    }
    iterator end() { return { p, true }; }
    generator( generator const & ) = delete;
    generator( generator && other ) noexcept
        : p { other.p }
    {
        other.p = nullptr;
    }
    ~generator()
    {
        if ( p ) p.destroy();
    }

private:
    explicit generator( promise_type * p )
        : p { std::coroutine_handle<promise_type>::from_promise( *p ) }
    {
    }
    std::coroutine_handle<promise_type> p;
};

template<typename T>
generator<T> seq()
{
    for ( T i = {};; ++i ) {
        co_yield i;
    }
}

template<typename T>
generator<T> take_until( generator<T> & g, T limit )
{
    for ( auto && v : g ) {
        if ( v < limit )
            co_yield v;
        else
            break;
    }
}

template<typename T>
generator<T> multiply( generator<T> & g, T factor )
{
    for ( auto && v : g ) {
        co_yield v * factor;
    }
}

template<typename T>
generator<T> add( generator<T> & g, T adder )
{
    for ( auto && v : g ) {
        co_yield v + adder;
    }
}

int main()
{
    auto s = seq<int>;
    auto t = take_until<int>( s, 10 );
    auto m = multiply( t, 2 );
    auto a = add( m, 110 );
    return std::accumulate( a.begin(), a.end(), 0 );
}
