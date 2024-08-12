#include <atomic>
#include <thread>

#include "spinlock.h"

Spinlock spin;

void workOnResource()
{
    spin.lock();
    // shared resource
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