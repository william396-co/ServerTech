#include <chrono>
#include <future>
#include <iostream>

int main()
{

    auto begin = std::chrono::system_clock::now();

    auto asyncLazy = std::async( std::launch::deferred, [] { return std::chrono::system_clock::now(); } );

    auto asyncEager = std::async( std::launch::async, [] { return std::chrono::system_clock::now(); } );

    std::this_thread::sleep_for( std::chrono::milliseconds { 1 } );

    auto lazyStart = asyncLazy.get() - begin;

    auto eagerStart = asyncEager.get() - begin;

    auto lazyDuration = std::chrono::duration_cast<std::chrono::microseconds>( lazyStart ).count();
    auto eagerDuration = std::chrono::duration_cast<std::chrono::microseconds>( eagerStart ).count();

    std::cout << "asyncLazy evaluated after: " << lazyDuration << " microseconds.\n";

    std::cout << "asyncEager evaluated after: " << eagerDuration << " microseconds.\n";

    return 0;
}
