#include <memory>
#include <iostream>

int main()
{

    std::unique_ptr<int> p( new int { 42 } );

    /* compiler error
    [p]() {
        std::cout << *p << "\n";
    }();*/

    // compiles, but the ownership is not passed
    [&p]() {
        std::cout << *p << "\n";
    }();

    return 0;
}
