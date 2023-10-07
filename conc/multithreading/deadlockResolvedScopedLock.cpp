#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>

struct CriticalData
{
    std::mutex mut;
};

void deadLock( CriticalData & a, CriticalData & b )
{

    std::cout << "thread: " << std::this_thread::get_id() << " first mutex\n";
    std::this_thread::sleep_for( std::chrono::milliseconds { 1 } );

    std::cout << "thread: " << std::this_thread::get_id() << " second mutex\n";

    std::cout << "   Thread: " << std::this_thread::get_id() << " get both mutex\n";

    std::scoped_lock( a.mut, b.mut );
}

int main()
{

    CriticalData c1;
    CriticalData c2;
    std::thread t1( [&] { deadLock( c1, c2 ); } );
    std::thread t2( [&] { deadLock( c2, c1 ); } );

    t1.join();
    t2.join();

    return 0;
}
