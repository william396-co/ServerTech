#include "acquire_release_opt.h"

#include <atomic>
#include <thread>
#include <cassert>

#include "print.h"

namespace acquire_release_opt {

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store( true, std::memory_order_relaxed );
    y.store( true, std::memory_order_release );
}

void read_y_then_x()
{
    while ( !y.load( std::memory_order_acquire ) )
        ;
    if ( x.load( std::memory_order_relaxed ) )
        ++z;
}

void example()
{
    println( __PRETTY_FUNCTION__ );

    x = false;
    y = false;
    z = 0;
    std::thread a( write_x_then_y );
    std::thread d( read_y_then_x );

    a.join();
    d.join();

    assert( z.load() != 0 );
    println( "z=", z );
}
} // namespace acquire_release_opt
