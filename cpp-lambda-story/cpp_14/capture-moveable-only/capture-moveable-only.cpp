#include <iostream>
#include <memory>

int main()
{

    std::unique_ptr<int> p( new int { 10 } );

    // Thanks to the capture initialiser you can move the pointer ownership into lambda
    const auto bar = [ptr = std::move( p )] {
        std::cout << "pointer in lambda:" << ptr.get() << "\n"; // 0xbadeb0
    };
    std::cout << "pointer in main():" << p.get() << "\n"; // 0

    bar();
    bar();
    return 0;
}
