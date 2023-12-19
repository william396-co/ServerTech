#include <future>
#include <thread>
#include <iostream>

int main()
{

    int res;
    std::thread t( [&] { res = 2000 + 11; } );
    t.join();

    std::cout << "res: " << res << "\n";

    auto fut = std::async( []( int i ) { return 2000 + i; }, 11 );
    std::cout << "fut.get(): " << fut.get() << "\n";

    return 0;
}
