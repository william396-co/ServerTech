#include "atomic_example.h"

#include <atomic>
#include <thread>
#include <iostream>

void atomic_example()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    std::atomic<bool> b { true };
    std::cout << std::boolalpha;

    std::cout << "b=" << b << "\n";
}
