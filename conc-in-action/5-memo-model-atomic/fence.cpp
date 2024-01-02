#include "fence.h"

#include <atomic>
#include <thread>
#include <string>
#include <cassert>

#include "print.h"

namespace fence {

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store( true, std::memory_order_relaxed );
    std::atomic_thread_fence( std::memory_order_release );
    y.store( true, std::memory_order_relaxed );
}

void read_y_then_x()
{
    while ( !y.load( std::memory_order_relaxed ) )
        ;
    std::atomic_thread_fence( std::memory_order_acquire );
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
    std::thread b( read_y_then_x );

    a.join();
    b.join();

    assert( z.load() != 0 );
    println( std::boolalpha, "x = ", x, " y = ", y, " z = ", z );
}
} // namespace fence
