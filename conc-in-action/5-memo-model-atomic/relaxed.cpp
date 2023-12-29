#include "relaxed.h"

#include <atomic>
#include <thread>
#include <cassert>

#include "print.h"

namespace relaxed {
std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store( true, std::memory_order_relaxed );
    y.store( true, std::memory_order_relaxed );
}

void read_y_then_x()
{
    while ( !y.load( std::memory_order_relaxed ) )
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
    assert( x.load() != 0 );

    println( "z=", z );
}
} // namespace relaxed
