#include "fence_2.h"

#include <atomic>
#include <thread>
#include <cassert>

#include "print.h"

namespace fence_2 {

bool x = false;
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y()
{
    x = true;
    std::atomic_thread_fence( std::memory_order_release );
    y.store( true, std::memory_order_relaxed );
}

void read_y_then_x()
{
    while ( !y.load( std::memory_order_relaxed ) )
        ;
    std::atomic_thread_fence( std::memory_order_acquire );
    if ( x )
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
}
} // namespace fence_2
