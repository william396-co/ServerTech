#include "spinlock.h"

#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>
#include <functional>

void spinlock_example()
{
    int count = 0;
    spinlock mtx {};

    std::vector<std::thread> threads;
    for ( size_t i = 0; i != 100; ++i ) {
        threads.emplace_back(
            [&]( auto idx ) {
                std::lock_guard lock( mtx );
                count += idx;
            },
            i );
    }

    std::for_each( threads.begin(), threads.end(), std::mem_fn( &std::thread::join ) );

    std::cout << __PRETTY_FUNCTION__ << " count = " << count << "\n";
}
