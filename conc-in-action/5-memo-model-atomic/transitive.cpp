#include "transitive.h"

#include <atomic>
#include <thread>
#include <cassert>
#include "print.h"

namespace transitive {

std::atomic<int> data[5];
std::atomic<bool> sync1 { false }, sync2 { false };

void t1()
{
    data[0].store( 42, std::memory_order_relaxed );
    data[1].store( 97, std::memory_order_relaxed );
    data[2].store( 17, std::memory_order_relaxed );
    data[3].store( -141, std::memory_order_relaxed );
    data[4].store( 2003, std::memory_order_relaxed );
    sync1.store( true, std::memory_order_release );
}

void t2()
{
    while ( !sync1.load( std::memory_order_acquire ) )
        ;
    sync2.store( true, std::memory_order_release );
}

void t3()
{
    while ( !sync2.load( std::memory_order_acquire ) )
        ;

    assert( data[0].load( std::memory_order_relaxed ) == 42 );
    assert( data[1].load( std::memory_order_relaxed ) == 97 );
    assert( data[2].load( std::memory_order_relaxed ) == 17 );
    assert( data[3].load( std::memory_order_relaxed ) == -141 );
    assert( data[4].load( std::memory_order_relaxed ) == 2003 );
}

void example()
{
    println( __PRETTY_FUNCTION__ );

    std::thread a( t1 );
    std::thread b( t2 );
    std::thread c( t3 );

    a.join();
    b.join();
    c.join();
}
} // namespace transitive
