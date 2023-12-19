#include <iostream>
#include <thread>
#include <mutex>

int x = 0;
int y = 0;
std::mutex mut;

void writing()
{
    std::lock_guard lock( mut );
    x = 2000;
    y = 11;
}

void reading()
{
    std::lock_guard lock( mut );
    std::cout << "y: " << y << " x: " << x << "\n";
}

int main()
{
    std::thread t1( writing );
    std::thread t2( reading );

    t1.join();
    t2.join();

    return 0;
}
