#include <thread>
#include <atomic>
#include <iostream>

#include "lock_free_stack.h"

int main()
{
    std::cout << "Designing  lock-free Concurrent data structures\n";

    lock_free_stack_example();

    return 0;
}
