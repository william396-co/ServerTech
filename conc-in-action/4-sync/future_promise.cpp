#include "future_promise.h"

#include <map>
#include <string>
#include <stdexcept>
#include <cmath>
#include <future>
#include <cassert>

double square_root( double x )
{
    if ( x < 0 )
        throw std::out_of_range( "x<0" );
    return sqrt( x );
}

void future_exception_example()
{
    //    double y = square_root(-1);

    std::future<double> f = std::async( square_root, -1 );
    double y = f.get();
}

double calc_value()
{
    return 42;
}

std::promise<double> some_promise;

void handle_promise_exception()
{
    try {
        some_promise.set_value( calc_value() );
    } catch ( ... ) {
        some_promise.set_exception( std::current_exception() );
    }
}

void shared_future_example()
{
    std::promise<int> p;
    std::future<int> f( p.get_future() );
    assert( f.valid() ); // the future is valid

    std::shared_future<int> sf( std::move( f ) );
    assert( !f.valid() ); // f is no longer valid
    assert( sf.valid() ); // sf is valid

    std::promise<std::string> sp;
    std::shared_future<std::string> sf2( sp.get_future() ); // implicit transfer of ownership

    assert( sf2.valid() );
    auto sf3 = std::move( sf2 );
    assert( !sf2.valid() );
    assert( sf3.valid() );

    std::promise<std::map<int, std::string>> pmp;

    auto lsf1 = pmp.get_future().share();
    auto lsf2 = pmp.get_future().share();
    auto lsf3 = pmp.get_future().share();
//    assert( lsf1.valid() );
  //  assert( lsf2.valid() );
    assert( lsf3.valid() );
}

