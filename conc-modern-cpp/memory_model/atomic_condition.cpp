#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> mySharedWork;
std::atomic<bool> dataReady( false );

void waitingForWork()
{
    std::cout << "waiting\n";
    while ( !dataReady.load() ) {
        std::this_thread::sleep_for( std::chrono::milliseconds { 5 } );
    }
    mySharedWork[1] = 2;
    std::cout << "Work done\n";
}

void setDataReady()
{
    mySharedWork = { 1, 0, 3 };
    dataReady = true;
    std::cout << "data prepared\n";
}

int main()
{

    std::thread t1( waitingForWork );
    std::thread t2( setDataReady );

    t1.join();
    t2.join();

    for ( auto const & v : mySharedWork ) {
        std::cout << v << "\n";
    }

    return 0;
}
