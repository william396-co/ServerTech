#pragma once

#include <atomic>
#include <thread>

class Spinlock
{
public:
    void lock()
    {
        while ( flag.test_and_set() )
            ;
    }
    void unlock()
    {
        flag.clear();
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};
