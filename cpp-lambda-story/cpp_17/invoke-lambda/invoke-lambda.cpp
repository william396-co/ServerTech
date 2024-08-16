#include <iostream>
#include <functional>
#include <string>

// IIFE- Immediately Invoked Functional Expression

int main()
{

    std::string name = "jack";
#define USE_CPP17
#ifndef USE_CPP17
    [&name]() {
        std::cout << "Hello " << name << "\n";
    }();
#else
    std::invoke( [&name]() {
        std::cout << "Hello " << name << "\n";
    } );

#endif
    return 0;
}
