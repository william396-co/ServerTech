#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <functional>

std::atomic<int> count { 0 };

void add()
{
    for ( int i = 0; i != 10000; ++i ) {
        count.fetch_add( 1, std::memory_order_relaxed );
    }
}

int main()
{
    std::vector<std::thread> threads;

    for ( int i = 0; i != 10; ++i ) {
        threads.emplace_back( add );
    }

    std::for_each( threads.begin(), threads.end(), std::mem_fn( &std::thread::join ) );

    std::cout << "Final counter value is " << count << "\n";

    return 0;
}
