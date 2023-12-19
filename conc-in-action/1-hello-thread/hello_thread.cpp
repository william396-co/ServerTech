#include <thread>
#include <iostream>

int main()
{
    std::thread t( [] {
        std::cout << "[" << std::this_thread::get_id() << "] hello thread\n";
    } );

    std::cout << "[" << std::this_thread::get_id() << "] Main thread\n";
    t.join();

    return 0;
}
