#include <iostream>

int main()
{

    auto foo = []( int x ) {
        if ( x < 0 )
            return x * 1.1f; // float
        else
            return x * 2.1; // double
    };

    std::cout << foo( -1 ) << "\n";
    std::cout << foo( 10 ) << "\n";

    return 0;
}
