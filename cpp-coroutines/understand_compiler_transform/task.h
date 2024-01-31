#pragma once

#include <utility>
#include <memory>
#include <type_traits>
#include <concepts>
#include <cstddef>
#include <utility>

struct __coroutine_state
{
    using __resume_fn = __coroutine_state *( __coroutine_state * );
    using __destroy_fn = void( __coroutine_state * );

    __resume_fn * __resume;
    __destroy_fn * __destroy;

    static __coroutine_state * __noop_resume( __coroutine_state * __state ) noexcept
    {
        return __state;
    }
    static void __noop_destroy( __coroutine_state * ) noexcept
    {
    }

    static const __coroutine_state __noop_coroutine;
};

inline const __coroutine_state __coroutine_state::__noop_coroutine {
    &__coroutine_state::__noop_resume, &__coroutine_state::__noop_destroy
};

template<typename Promise>
struct __coroutine_state_with_promise : __coroutine_state
{
    __coroutine_state_with_promise() noexcept {}
    ~__coroutine_state_with_promise() {}

    union
    {
        Promise __promise;
    };
};
namespace std {

template<typename Ret, typename... Args>
struct coroutine_traits
{
    using promise_type = typename std::remove_cvref_t<Ret>::promise_type;
};

template<typename Promise = void>
class coroutine_handle;

template<>
class coroutine_handle<void>
{
public:
    coroutine_handle() noexcept = default;
    coroutine_handle( coroutine_handle const & ) noexcept = default;
    coroutine_handle & operator=( coroutine_handle const & ) noexcept = default;

    void * address() const
    {
        return static_cast<void *>( state_ );
    }
    static coroutine_handle from_address( void * ptr )
    {
        coroutine_handle h;
        h.state_ = static_cast<__coroutine_state *>( ptr );
        return h;
    }

    explicit operator bool() noexcept
    {
        return state_ != nullptr;
    }

    friend bool operator==( coroutine_handle const & a, coroutine_handle const & b ) noexcept
    {
        return a.state_ == b.state_;
    }

    void resume() const
    {
        __coroutine_state * s = state_;
        do {
            s = s->__resume( s );
        } while ( s != &__coroutine_state::__noop_coroutine );
    }

    void destroy() const
    {
        state_->__destroy( state_ );
    }

    bool done() const
    {
        return state_->__resume == nullptr;
    }

private:
    __coroutine_state * state_ {};
};

template<typename Promise>
class coroutine_handle
{
    using state_t = __coroutine_state_with_promise<Promise>;

public:
    coroutine_handle() noexcept = default;
    coroutine_handle( coroutine_handle const & ) noexcept = default;
    coroutine_handle & operator=( coroutine_handle const & ) noexcept = default;

    operator coroutine_handle<void>() const noexcept
    {
        return coroutine_handle<void>::from_address( address() );
    }

    explicit operator bool() const noexcept
    {
        return state_ != nullptr;
    }
    friend bool operator==( coroutine_handle const & a, coroutine_handle const & b ) noexcept
    {
        return a.state_ == b.state_;
    }

    void * address() const
    {
        return static_cast<void *>( static_cast<__coroutine_state *>( state_ ) );
    }

    static coroutine_handle from_address( void * ptr )
    {
        coroutine_handle h;
        h.state_ = static_cast<state_t *>( static_cast<__coroutine_state *>( ptr ) );
    }

    Promise & promise() const
    {
        return state_->__promise;
    }

    static coroutine_handle from_promise( Promise & promise )
    {
        coroutine_handle h;
        // we know the address of the __promise member, so calculate the
        // address of the coroutine-state by substracting the offset of
        // the __promise failed from this address
        h.state_ = reinterpret_cast<state_t *>(
            reinterpret_cast<unsigned char *>( std::addressof( promise ) ) - offsetof( state_t, __promise ) );
        return h;
    }

    // Define these in terms their coroutine_handle<void> implementations
    void resume() const
    {
        static_cast<coroutine_handle<void>>( *this ).resume();
    }
    void destroy() const
    {
        static_cast<coroutine_handle<void>>( *this ).destroy();
    }
    bool done() const
    {
        return static_cast<coroutine_handle<void>>( *this ).done();
    }

private:
    state_t * state_;
};

struct noop_coroutine_promise
{};
using noop_coroutine_handle = coroutine_handle<noop_coroutine_promise>;
noop_coroutine_handle noop_coroutine() noexcept;

template<>
class coroutine_handle<noop_coroutine_promise>
{
public:
    constexpr coroutine_handle( coroutine_handle const & ) noexcept = default;
    constexpr coroutine_handle & operator=( coroutine_handle const & ) noexcept = default;

    constexpr explicit operator bool() noexcept { return true; }

    constexpr friend bool operator==( coroutine_handle const &, coroutine_handle const & ) noexcept { return true; }

    operator coroutine_handle<void>() const noexcept
    {
        return coroutine_handle<void>::from_address( address() );
    }

    noop_coroutine_promise & promise() const noexcept
    {
        static noop_coroutine_promise promise;
        return promise;
    }

    constexpr void resume() const noexcept {}
    constexpr void destroy() const noexcept {}
    constexpr bool done() const noexcept { return false; }

    constexpr void * address() const noexcept
    {
        return const_cast<__coroutine_state *>( &__coroutine_state::__noop_coroutine );
    }

private:
    constexpr coroutine_handle() noexcept = default;

    friend noop_coroutine_handle noop_coroutine() noexcept
    {
        return {};
    }
};

struct suspend_always
{
    constexpr suspend_always() noexcept = default;
    constexpr bool await_ready() const noexcept
    {
        return false;
    }
    constexpr void await_suspend( coroutine_handle<> ) const noexcept {}
    constexpr void await_resume() const noexcept {}
};

} // namespace std

#define ENABLE_TASK_DEFINITIONS
#ifdef ENABLE_TASK_DEFINITIONS
#include <exception>
#include <variant>
class task
{
public:
    struct awaiter;
    class promise_type
    {
    public:
        promise_type() noexcept;
        ~promise_type();

        struct final_awaiter
        {
            bool await_ready() noexcept;
            std::coroutine_handle<> await_suspend( std::coroutine_handle<promise_type> h ) noexcept;
            void await_resume() noexcept;
        };

        task get_return_object() noexcept;
        std::suspend_always initial_suspend() noexcept;
        final_awaiter final_suspend() noexcept;
        void unhandled_exception() noexcept;
        void return_value( int result ) noexcept;

    private:
        friend awaiter;
        std::coroutine_handle<> continuation_;
        std::variant<std::monostate, int, std::exception_ptr> result_;
    };

    task( task && t ) noexcept;
    ~task();
    task & operator=( task && t ) noexcept;

    struct awaiter
    {
        explicit awaiter( std::coroutine_handle<promise_type> h ) noexcept;
        bool await_ready() noexcept;
        std::coroutine_handle<promise_type> await_suspend( std::coroutine_handle<> h ) noexcept;
        int await_resume();

    private:
        std::coroutine_handle<promise_type> coro_;
    };

    awaiter operator co_await() && noexcept;

private:
    explicit task( std::coroutine_handle<promise_type> h ) noexcept;
    std::coroutine_handle<promise_type> coro_;
};

inline task::promise_type::promise_type() noexcept {}
inline task::promise_type::~promise_type() {}

inline bool task::promise_type::final_awaiter::await_ready() noexcept
{
    return false;
}

inline std::coroutine_handle<> task::promise_type::final_awaiter::await_suspend( std::coroutine_handle<task::promise_type> h ) noexcept
{
    return h.promise().continuation_;
}

inline void task::promise_type::final_awaiter::await_resume() noexcept {}

inline task task::promise_type::get_return_object() noexcept
{
    return task { std::coroutine_handle<task::promise_type>::from_promise( *this ) };
}

inline std::suspend_always task::promise_type::initial_suspend() noexcept
{
    return {};
}

inline task::promise_type::final_awaiter task::promise_type::final_suspend() noexcept
{
    return {};
}

inline void task::promise_type::unhandled_exception() noexcept
{
    result_.emplace<2>( std::current_exception() );
}

inline void task::promise_type::return_value( int value ) noexcept
{
    result_.emplace<1>( value );
}

inline task::task( task && t ) noexcept
    : coro_( std::exchange( t.coro_, {} ) ) {}

inline task::~task()
{
    if ( coro_ ) {
        coro_.destroy();
    }
}

inline task & task::operator=( task && t ) noexcept
{
    task tmp = std::move( t );
    std::swap( coro_, tmp.coro_ );
    return *this;
}

inline task::awaiter::awaiter( std::coroutine_handle<promise_type> h ) noexcept
    : coro_ { h }
{
}

inline bool task::awaiter::await_ready() noexcept
{
    return false;
}

inline std::coroutine_handle<task::promise_type> task::awaiter::await_suspend( std::coroutine_handle<> h ) noexcept
{
    coro_.promise().continuation_ = h;
    return coro_;
}

inline int task::awaiter::await_resume()
{
    if ( coro_.promise().result_.index() == 2 ) {
        std::rethrow_exception( std::get<2>( std::move( coro_.promise().result_ ) ) );
    }
    return std::get<1>( coro_.promise().result_ );
}

inline task::awaiter task::operator co_await() && noexcept
{
    return task::awaiter { coro_ };
}

inline task::task( std::coroutine_handle<task::promise_type> h ) noexcept
    : coro_ { h } {}

#endif

task f( int x );

template<typename T>
struct manual_lifetime
{
    manual_lifetime() noexcept = default;
    ~manual_lifetime() = default;

    // Not copyable/movable
    manual_lifetime( manual_lifetime const & ) = delete;
    manual_lifetime( manual_lifetime && ) = delete;
    manual_lifetime & operator=( manual_lifetime const & ) = delete;
    manual_lifetime & operator=( manual_lifetime && ) = delete;

    template<typename Factory>
        requires std::invocable<Factory &> && std::same_as<std::invoke_result_t<Factory &>, T>
    T & construct_from( Factory factory ) noexcept( std::is_nothrow_invocable_v<Factory &> )
    {
        return *::new ( static_cast<void *>( &storage ) ) T( factory() );
    }

    void destroy() noexcept( std::is_nothrow_destructible_v<T> )
    {
        std::destroy_at( std::launder( reinterpret_cast<T *>( &storage ) ) );
    }

    T & get() noexcept
    {
        return *std::launder( reinterpret_cast<T *>( &storage ) );
    }

private:
    alignas( T ) std::byte storage[sizeof( T )];
};

template<typename T>
struct destructor_guard
{
    explicit destructor_guard( manual_lifetime<T> & obj ) noexcept
        : ptr_( std::addressof( obj ) ) {}

    // non-movable
    destructor_guard( destructor_guard && ) = delete;
    destructor_guard & operator=( destructor_guard && ) = delete;

    ~destructor_guard() noexcept( std::is_nothrow_destructible_v<T> )
    {
        if ( ptr_ ) {
            ptr_->destroy();
        }
    }

    void cancel() noexcept { ptr_ = nullptr; }

private:
    manual_lifetime<T> * ptr_;
};

// Partial specialisation for types that don't need their destructors called
template<typename T>
    requires std::is_trivially_destructible_v<T>
struct destructor_guard<T>
{
    explicit destructor_guard( manual_lifetime<T> & ) noexcept {}
    void cancel() noexcept {}
};

// Class-template argument deduction to simplify usage
template<typename T>
destructor_guard( manual_lifetime<T> & obj ) -> destructor_guard<T>;

template<typename Promise, typename... Args>
Promise construct_promise( [[maybe_unused]] Args &&... args )
{
    if constexpr ( std::constructible_from<Promise, Args &...> ) {
        return Promise( std::forward<Args &&>( args )... );
    } else {
        return Promise();
    }
}

using __g_promise_t = std::coroutine_traits<task, int>::promise_type;
__coroutine_state * __g_resume( __coroutine_state * s );
void __g_destroy( __coroutine_state * s );

struct __g_state : __coroutine_state_with_promise<__g_promise_t>
{

    __g_state( int && __x )
        : x( static_cast<int &&>( __x ) )
    {
        // use placement-new to initialise the promise object in the base-class
        ::new ( (void *)std::addressof( this->__promise ) ) __g_promise_t( construct_promise<__g_promise_t>( x ) );
    }

    ~__g_state()
    {
        // Also need to manually call the promise destructor before the
        // argment objects are destroyed
        this->__promise.~__g_promise_t();
    }

    int __suspend_point = 0;
    int x;
    struct __scope1
    {
        manual_lifetime<task> __tmp2;
        manual_lifetime<task::awaiter> __tmp3;
    };
    union
    {
        manual_lifetime<std::suspend_always> __tmp1;
        __scope1 __s1;
        manual_lifetime<task::promise_type::final_awaiter> __tmp4;
    };
};

task g( int x )
{
    std::unique_ptr<__g_state> state( new __g_state( static_cast<int &&>( x ) ) );
    decltype( auto ) return_value = state->__promise.get_return_object();

    state->__tmp1.construct_from( [&]() -> decltype( auto ) {
        return state->__promise.initial_suspend();
    } );

    if ( !state->__tmp1.get().await_ready() ) {
        state->__tmp1.get().await_suspend(
            std::coroutine_handle<__g_promise_t>::from_promise( state->__promise ) );
        state.release();
        // fall through to return statement below
    } else {
        // Coroutine did not suspend, Start excuting the body immediately
        __g_resume( state.release() );
    }

    return return_value;
}

__coroutine_state * __g_resume( __coroutine_state * s )
{
    // we know that's' points to a __g_state
    auto * state = static_cast<__g_state *>( s );

    try {

        // generate a jump-table to jump to the correct place in the code based
        // on the value of the suspend-point index
        switch ( state->__suspend_point ) {
            case 0: goto suspend_point_0;
            case 1: goto suspend_point_1;
            default: // std::unreachable();
                goto final_suspend;
        }

    suspend_point_0: {
        destructor_guard tmp1_dtor { state->__tmp1 };
        state->__tmp1.get().await_resume();
    }

        {
            // int fx = co_await f(x);
            state->__s1.__tmp2.construct_from( [&] {
                return f( state->x );
            } );
            destructor_guard tmp2_dtor { state->__s1.__tmp2 };

            state->__s1.__tmp3.construct_from( [&] {
                return static_cast<task &&>( state->__s1.__tmp2.get() ).operator co_await();
            } );
            destructor_guard tmp3_dtor { state->__s1.__tmp3 };

            if ( !state->__s1.__tmp3.get().await_ready() ) {
                // mark the suspend-point
                state->__suspend_point = 1;

                auto h = state->__s1.__tmp3.get().await_suspend(
                    std::coroutine_handle<__g_promise_t>::from_promise( state->__promise ) );

                // A coroutine suspends without exiting scopes
                // So cancel the destructor-gurads
                tmp3_dtor.cancel();
                tmp2_dtor.cancel();

                return static_cast<__coroutine_state *>( h.address() );
            }

            // Don't exit the scope here
            // We can't 'goto' a label that enters the scope of a variable with a
            // non-trivial destructor, So we have to exit the scope of the destructor
            // guards here without calling the destructors and then recreate them after
            // the 'suspend_point_1' label.
            tmp3_dtor.cancel();
            tmp2_dtor.cancel();
        }

    suspend_point_1:
        int fx = [&]() -> decltype( auto ) {
            destructor_guard tmp2_dtor { state->__s1.__tmp2 };
            destructor_guard tmp3_dtor { state->__s1.__tmp3 };
            return state->__s1.__tmp3.get().await_resume();
        }();
        // co_return fx * fx
        state->__promise.return_value( fx * fx );
        goto final_suspend;
    } catch ( ... ) {
        state->__promise.unhandled_exception();
        goto final_suspend;
    }
final_suspend: {
    state->__tmp4.construct_from( [&]() noexcept {
        return state->__promise.final_suspend();
    } );
    destructor_guard tmp4_dtor { state->__tmp4 };

    if ( !state->__tmp4.get().await_ready() ) {
        state->__suspend_point = 2;
        state->__resume = nullptr; // mark as final suspend-point(can not resume again)

        auto h = state->__tmp4.get().await_suspend(
            std::coroutine_handle<__g_promise_t>::from_promise( state->__promise ) );

        tmp4_dtor.cancel();
        return static_cast<__coroutine_state *>( h.address() );
    }

    state->__tmp4.get().await_resume();
}
    // destrory coroutine_state if execution flows off end of coroutine
    delete state;
    return static_cast<__coroutine_state *>( std::noop_coroutine().address() );
}

void __g_destroy( __coroutine_state * s )
{
    auto * state = static_cast<__g_state *>( s );
    switch ( state->__suspend_point ) {
        case 0: goto suspend_point_0;
        case 1: goto suspend_point_1;
        case 2: goto suspend_point_2;
        default: // std::unreachable(); C++23
            goto destroy_state;
    }

suspend_point_0:
    state->__tmp1.destroy();
    goto destroy_state;

suspend_point_1:
    state->__s1.__tmp3.destroy();
    state->__s1.__tmp2.destroy();
    goto destroy_state;

suspend_point_2:
    state->__tmp4.destroy();
    goto destroy_state;

destroy_state:
    delete state;
}
