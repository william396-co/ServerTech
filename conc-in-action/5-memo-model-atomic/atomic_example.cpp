#include "atomic_example.h"

#include <atomic>
#include <thread>
#include <iostream>
#include <cassert>
#include <memory>

void atomic_example()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    std::atomic<bool> b { true };
    std::cout << std::boolalpha;

    std::cout << "b=" << b << "\n";

    bool x = b.load( std::memory_order_acquire );
    std::cout << "x = " << x << "\n";
    b.store( false );
    std::cout << "b = " << b << "\n";

    x = b.exchange( false, std::memory_order_acq_rel );
    std::cout << "b = " << b << " x = " << x << std::endl;
}

void atomic_ptr_example()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    Foo some_array[5];
    std::atomic<Foo *> p( some_array );
    Foo * x = p.fetch_add( 2 );
    assert( x == some_array );
    assert( p.load() == &some_array[2] );
    x = ( p -= 1 );
    assert( x == &some_array[1] );
    assert( p.load() == &some_array[1] );
}

struct my_data
{};

void process_data( std::shared_ptr<my_data> pd )
{
}

std::shared_ptr<my_data> p;
void process_global_data()
{
    std::shared_ptr<my_data> local = std::atomic_load( &p );
    process_data( local );
}

void update_global_data()
{
    std::shared_ptr<my_data> local( new my_data );
    std::atomic_store( &p, local );
}
