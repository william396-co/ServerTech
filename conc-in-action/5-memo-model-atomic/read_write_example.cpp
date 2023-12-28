#include "read_write_example.h"
#include <atomic>
#include <thread>
#include <vector>
#include <future>
#include <chrono>

#include "print.h"

std::vector<int> data;
std::atomic<bool> data_ready { false };

void reader_thread()
{
    while ( !data_ready.load() ) {
        std::this_thread::sleep_for( std::chrono::milliseconds { 1 } );
    }
    println( "The answer = ", data[0] );
}

void writer_thread()
{
    data.push_back( 42 );
    data_ready = true;
}

void read_write_example()
{
    println( __PRETTY_FUNCTION__ );

    auto f1 = std::async( reader_thread );
    auto f2 = std::async( writer_thread );
    f1.wait();
    f2.wait();
}
