#pragma once

#include <thread>
#include <atomic>
#include <iostream>
#include "print.h"

// #define USE_ATOMIC_FLAG
#define USE_CAS

#ifdef USE_ATOMIC_FLAG
class spinlock
{
public:
    spinlock()
        : flag { ATOMIC_FLAG_INIT } {}

    void lock()
    {
        bool b = { false };
        while ( b = flag.test_and_set( std::memory_order_acquire ) ) {
            printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " b = ", b, " flag = ", flag.test(), "================" );
        }
        printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " b = ", b, " flag = ", flag.test() );
    }
    void unlock()
    {
        flag.clear( std::memory_order_release );
        printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " flag = ", flag.test() );
    }

private:
    std::atomic_flag flag;
};

#else

#ifndef USE_CAS
class spinlock
{
public:
    spinlock()
        : flag { false } {}

    void lock()
    {
        bool b {};
        while ( b = flag.exchange( true, std::memory_order_acquire ) ) {
            printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " b = ", b, " flag = ", flag, "================" );
        }
        printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " b = ", b, " flag = ", flag );
    }
    void unlock()
    {
        flag.store( false, std::memory_order_release );
        printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " flag = ", flag );
    }

private:
    std::atomic<bool> flag;
};

#else

class spinlock
{
public:
    spinlock()
        : flag { false } {}
    void lock()
    {
        bool expected = false;
        printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " expected = ", expected, " flag = ", flag, "---------------" );
        while ( !flag.compare_exchange_weak( expected, true, std::memory_order_acquire ) ) {
            printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " expected = ", expected, " flag = ", flag, "================" );
            expected = false;
        }
        printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " expected = ", expected, " flag = ", flag );
    }

    void unlock()
    {
        flag.store( false, std::memory_order_release );
        printlnEx( std::this_thread::get_id(), " ", __PRETTY_FUNCTION__, std::boolalpha, " flag = ", flag );
    }

private:
    std::atomic<bool> flag;
};

#endif

#endif

void spinlock_example();
