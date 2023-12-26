#include "future_then.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <cassert>

int find_the_answer()
{
    std::cout << __PRETTY_FUNCTION__ << " start\n";
    std::this_thread::sleep_for( std::chrono::seconds { 3 } );
    std::cout << __PRETTY_FUNCTION__ << " end\n";
    return 42;
}

void find_the_question()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

/*
 * Continuation-style concurrency with the Concurrency TS
 * std::experimental::future.then(func)
 */
void future_then_example()
{
#ifdef CONC_TS
    std::experimental::future<int> find_the_answer;
    auto fut = find_the_answer();
    auto fut2 = fut.then( find_the_question );

    std::cout << " after fut2 \n";

    assert( !fut.valid() );
    assert( fut2.valid() );
#endif
}

Auth backend {};

#ifndef CONC_TS
void process_login( std::string const & username, std::string const & password )
{
    try {
        user_id const id = backend.authenticate_user( username, password );
        user_data const info_to_display = backend.request_current_info( id );
        update_display( info_to_display );

    } catch ( std::exception & e ) {
        display_error( e );
    }
}
#else
#ifdef ASYNC_OP
std::future<void> process_login( std::string const & username, std::string const & password )
{
    return std::async( std::launch::async, [=] {
        try {
            user_id const id = backend.authenticate_user( username, password );
            user_data const info_to_display = backend.request_current_info( id );
            update_display( info_to_display );

        } catch ( std::exception & e ) {
            display_error( e );
        }
    } );
}
#endif

#ifdef CONTINUES_OP
std::experimental::future<void> process_login( std::string const & username, std::string password )
{
    return spawn_async( [=]() {
        return backend.authenticate_user( username, password );
    } ).then( []( std::experimental::future<user_id> id ) {
           return backend.request_current_info( id.get() ); // user .get() to retrieve the contained value
       } )
        .then( []( std::experimental::future<user_data> info_to_display ) {
            try {
                update_display( info_to_display.get() );
            } catch ( std::exception & e ) {
                display_error( e );
            }
        } );
}

#endif

#endif
