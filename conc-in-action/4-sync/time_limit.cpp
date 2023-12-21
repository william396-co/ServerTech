#include "time_limit.h"

#include <iostream>
#include <future>
#include <thread>

using namespace std::chrono;
using namespace std::chrono_literals;

auto one_day = 24h;
auto half_an_hour = 30min;
auto max_time_between_messages = 30ms;

void chrono_example()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    std::chrono::milliseconds ms { 54802 };
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>( ms );

    std::cout << ms << "=" << s << "\n";

    std::cout << "one_day = " << one_day << "\n";
}

int some_task()
{
    std::this_thread::sleep_for( 50ms );
    std::cout << __PRETTY_FUNCTION__ << "\n";
    return 42;
}

void do_something_with( int n )
{
    std::cout << __PRETTY_FUNCTION__ << "(" << n << ")\n";
}

void future_wait_for_example()
{

    std::future<int> f = std::async( some_task );
    if ( f.wait_for( 35ms ) == std::future_status::ready ) {
        do_something_with( f.get() );
    } else {
        std::cout << "future wait time out!\n";
    }
}

void duration_example()
{
    auto start = std::chrono::high_resolution_clock::now();
    some_task();
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << " do some_task()  took " << std::chrono::duration_cast<std::chrono::milliseconds>( stop - start ).count()
              << " milliseconds\n";
}

#include <condition_variable>
#include <mutex>
#include <chrono>

std::condition_variable cv;
std::mutex m;
bool done = false;
bool wait_loop()
{
    auto const timeout = std::chrono::steady_clock::now() + 500ms;
    std::unique_lock lk( m );
    while ( !done ) {
        if ( cv.wait_until( lk, timeout ) == std::cv_status::timeout )
            break;
    }
    return done;
}
