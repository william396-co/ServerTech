#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> mySharedWork;
std::atomic<bool> dataProduced { false };
std::atomic<bool> dataConsumed { false };

void dataProducer()
{
    mySharedWork = { 1, 0, 3 };
    dataProduced.store( true, std::memory_order_release );
}

void deliveryBoy()
{
    while ( !dataProduced.load( std::memory_order_acquire ) ) {
        std::cout << __FUNCTION__ << " waiting...\n";
    }
    dataConsumed.store( true, std::memory_order_release );
}

void dataConsumer()
{

    while ( !dataConsumed.load( std::memory_order_acquire ) ) {
        std::cout << __FUNCTION__ << " waiting...\n";
    }

    mySharedWork[1] = 2;
}

int main()
{

    std::thread t1( dataProducer );
    std::thread t2( deliveryBoy );
    std::thread t3( dataConsumer );

    t1.join();
    t2.join();
    t3.join();

    for ( auto const & v : mySharedWork ) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    return 0;
}
