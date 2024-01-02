#include "release_sequence.h"

#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

#include "print.h"

namespace release_sequence {

std::vector<int> queue_data;
std::atomic<int> count;

void popluate_queue()
{
    constexpr auto number_of_items = 20;
    queue_data.clear();
    for ( size_t i = 0; i != number_of_items; ++i ) {
        queue_data.push_back( i );
        printlnEx( __PRETTY_FUNCTION__, "(", i, ")" );
        //  std::this_thread::sleep_for( std::chrono::microseconds { 1 } );
    }
    count.store( number_of_items, std::memory_order_release );
}

void process( int n )
{
    printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, "(", n, ")" );
}

void wait_for_more_items()
{
    printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__ );
}
void consume_queue_items()
{
    while ( true ) {
        int idx;
        if ( ( idx = count.fetch_sub( 1, std::memory_order_acquire ) ) <= 0 ) {
            if ( idx < 0 )
                break;
            wait_for_more_items();
            continue;
        }
        process( queue_data[idx - 1] );
    }
}

void example()
{
    std::thread a( popluate_queue );
    std::thread b( consume_queue_items );
    std::thread c( consume_queue_items );

    a.join();
    b.join();
    c.join();
}
} // namespace release_sequence
