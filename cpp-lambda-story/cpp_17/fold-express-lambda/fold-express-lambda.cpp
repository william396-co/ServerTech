#include <iostream>

int main()
{

    const auto sumLambda = []( auto... args ) {
        std::cout << "sum of:" << sizeof...( args ) << " numbers\n";

        return ( args + ... + 0 );
    };

    std::cout << sumLambda( 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 ) << "\n";

#define OPT
#ifndef OPT
    const auto printer = []( auto... args ) {
        ( std::cout << ... << args ) << "\n";
    };

#else
    const auto printer = []( auto... args ) {
        ( ( std::cout << args << "," ), ... );
        std::cout << "\n";
    };
#endif
    printer( 1, 2, 3, "hello", 10.4f );

    return 0;
}
