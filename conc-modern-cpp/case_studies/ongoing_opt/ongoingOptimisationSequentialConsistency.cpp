#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> x = { 0 };
std::atomic<int> y = { 0 };

void writing()
{
    x.store( 2000 );
    y.store( 11 );
}

void reading()
{
    std::cout << "y: " << y.load() << "";
    std::cout << " x: " << x.load() << "\n";
}

int main()
{
    std::thread t1( writing );
    std::thread t2( reading );

    t1.join();
    t2.join();

    return 0;
}
