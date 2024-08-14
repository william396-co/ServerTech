#include <iostream>

int main()
{

    const auto foo = []( auto x, int y ) {
        std::cout << x << ", " << y << "\n";
    };

    foo( 10, 1 );
    foo( 10.32434, 3 );
    foo( "hello world", 344 );

    return 0;
}
