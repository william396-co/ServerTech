#include <iostream>
#include <atomic>
#include <vector>
#include <thread>

std::atomic<int> x;
std::atomic<int> y;

void th1()
{
    x.store( 1, std::memory_order_release );
    auto res = y.load( std::memory_order_acquire );

    std::cout << "th1 x= " << x << " y= " << res << std::endl;
}

void th2()
{
    y.store( 1, std::memory_order_release );
    auto res = x.load( std::memory_order_acquire );

    std::cout << "th2 x= " << x << " y= " << res << std::endl;
}

int main()
{

    std::thread t1( th1 );
    std::thread t2( th2 );

    t1.join();
    t2.join();

    return 0;
}
