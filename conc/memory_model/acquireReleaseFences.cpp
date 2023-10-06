#include <atomic>
#include <thread>
#include <iostream>
#include <string>

std::atomic<std::string *> ptr;
int data;
std::atomic<int> atoData;

void producer()
{
    std::string * p = new std::string( "C++11" );
    data = 2011;
    atoData.store( 2014, std::memory_order_relaxed );
    std::atomic_thread_fence( std::memory_order_release );
    ptr.store( p, std::memory_order_relaxed );
}

void consumer()
{
    std::string * p;
    while ( !( p = ptr.load( std::memory_order_relaxed ) ) ) {
        std::cout << "waiting...\n";
    }
    std::atomic_thread_fence( std::memory_order_acquire );

    std::cout << "*p:" << *p << "\n";
    std::cout << "data:" << data << "\n";
    std::cout << "atoData:" << atoData.load( std::memory_order_relaxed ) << "\n";
}

int main()
{
    std::thread t1( producer );
    std::thread t2( consumer );

    t1.join();
    t2.join();

    return 0;
}
