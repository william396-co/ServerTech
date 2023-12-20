#include "async_example.h"

#include <functional>

move_only::move_only()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void move_only::operator()()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

X baz( X & x )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    return X {};
}

void async_example()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    X x;
    auto f1 = std::async( &X::foo, &x, 42, "hello" );
    auto f2 = std::async( &X::bar, &x, "goodbye" );

    Y y;
    auto f3 = std::async( Y(), 3.141 );
    auto f4 = std::async( std::ref( y ), 2.718 );

    // std::async( baz, std::ref( x ) );

    auto f5 = std::async( move_only {} );
}

void async_example2()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    X x;
    auto f6 = std::async( std::launch::async, Y(), 1.2 );              // run in new thread
    auto f7 = std::async( std::launch::deferred, baz, std::ref( x ) ); // run in wait() or  get()
    auto f8 = std::async( std::launch::deferred | std::launch::async, baz, std::ref( x ) );
    auto f9 = std::async( baz, std::ref( x ) );

    f7.wait();
}

