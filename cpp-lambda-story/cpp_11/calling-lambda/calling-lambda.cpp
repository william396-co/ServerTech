#include <iostream>

int main()
{
    // IIFE --Immediately Invoked Function Expression
    int x = 1, y = 1;
    [&]() noexcept {++x; ++y ; }(); // <-- call()
    std::cout << x << "," << y << "\n";
}
