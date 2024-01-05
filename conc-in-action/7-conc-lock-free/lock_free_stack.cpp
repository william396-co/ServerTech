#include "lock_free_stack.h"

#include <chrono>
using namespace std::chrono;
#include <random>
#include <thread>
#include "print.h"

lock_free_stack<int> lfs;

void push_work()
{
    while ( true ) {
        int value = std::rand() % 100;
        printlnEx( __PRETTY_FUNCTION__, "(", value, ")\n" );

        lfs.push( value );
    }
}

void pop_work()
{
    while ( true ) {
        auto value = lfs.pop();
        if ( value ) {
            printlnEx( __PRETTY_FUNCTION__, "(", *value, ")\n" );
        }
    }
}

void lock_free_stack_example()
{
    std::srand( duration_cast<seconds>( system_clock::now().time_since_epoch() ).count() );

    println( __PRETTY_FUNCTION__ );
    std::thread a( push_work );
    std::thread b( pop_work );
    a.join();
    b.join();
}
