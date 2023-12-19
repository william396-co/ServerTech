#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> x = { 0 };
std::atomic<int> y = { 0 };

void writing()
{
    x.store( 2000, std::memory_order_relaxed );
    y.store( 11, std::memory_order_release );
}

void reading()
{
    std::cout << "y: " << y.load( std::memory_order_acquire ) << "";
    std::cout << " x: " << x.load( std::memory_order_relaxed ) << "\n";
}

int main()
{
    std::thread t1( writing );
    std::thread t2( reading );

    t1.join();
    t2.join();

    return 0;
}
