#include <atomic>
#include <thread>
#include <chrono>

#include "spinlock.h"

Spinlock spin;

void workOnResource()
{

    spin.lock();
    std::this_thread::sleep_for( std::chrono::milliseconds { 2000 } );
    spin.unlock();
}

int main()
{

    std::thread t1( workOnResource );
    std::thread t2( workOnResource );

    t1.join();
    t2.join();
    return 0;
}
