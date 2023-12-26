#pragma once

#include <cstdint>
#include <string>
#include <iostream>

//#define CONTINUES_OP

#ifdef CONTINUES_OP
#include <experimental/future>
#else
#include <future>
#endif
#include <thread>
#include <exception>

#ifdef CONTINUES_OP
template<tpename Func>
std::experimental::future<decltype( std::declval<Func>()() )>
spawn_async( Func && func )
{
    std::experimental::promise<decltype( std::declval<Func>()() )> p;
    auto res = p.get_future();

    std::thread t( [p = std::move( p ), f = std::decay_t<Func>( func )]() mutable {
        try {
            p.set_value_at_thread_exit( f() );
        } catch ( ... ) {
            p.set_expcetion_at_thread_exit( std::current_exception() );
        }
    } );
    t.detach();
    return res;
}

#endif

void future_then_example();
#ifndef CONC_TS
void process_login( std::string const & username, std::string const & password );
#else
#ifdef ASYNC_OP
std::future<void> process_login( std::string const & username, std::string const & password );
#endif
#ifdef CONTINUES_OP
std::experimental::future<void> process_login( std::string const & username, std::string const & password );
#endif
#endif

using user_id = uint64_t;

struct user_data
{
    user_id id;
};

class Auth
{
public:
    user_id authenticate_user( std::string const & username, std::string const & password )
    {
        std::cout << __PRETTY_FUNCTION__ << " username:" << username << " pwd:" << password << "\n";
        return { 10011 };
    }
    user_data request_current_info( user_id id )
    {
        std::cout << __PRETTY_FUNCTION__ << " userid:" << id << "\n";
        return { id };
    }
};

inline void update_display( user_data const & info )
{
    std::cout << __PRETTY_FUNCTION__ << " userid:" << info.id << "\n";
}
inline void display_error( std::exception const & e )
{
    std::cout << __PRETTY_FUNCTION__ << "[ " << e.what() << "]\n";
}
