#include <string>
#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>
#include <mutex>

#include "spinlock.h"

struct S
{
    char a;     // #1
    int b : 5;  // #2
    int c : 11, // #2 (continued)
        : 0,
        d : 8;     // #3
    int e;         // #4
    double f;      // #5
    std::string g; // serveral memory locations
};

//Spinlock spin;
std::mutex spin;

void work( int n )
{
    spin.lock();
    std::this_thread::sleep_for( std::chrono::milliseconds { 2000 } );
    std::cout << std::this_thread::get_id() << " idx = " << n << " is working\n";
    spin.unlock();
}

int main()
{

    S s;

    //    if ( std::atomic<S>::is_always_lock_free )
    //        assert( std::atomic<S>().is_lock_free() );

    std::vector<std::thread> threads;
    for ( int i = 0; i != 10; ++i ) {
        threads.emplace_back( work, i );
    }

    std::for_each( threads.begin(), threads.end(), std::mem_fn( &std::thread::join ) );

    return 0;
}
