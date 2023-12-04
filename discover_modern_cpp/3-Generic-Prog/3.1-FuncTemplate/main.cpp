#include <iostream>

template<typename T>
T max( T a, T b )
{
    return a > b ? a : b;
}

int main()
{

    std::cout << "The maximum of 3 and 5 is " << max( 3, 5 ) << '\n';
    std::cout << "The maximum of 3l and 5l is " << max( 3l, 5l ) << "\n";
    std::cout << "The maximum of 3.0 and 5.0 is " << max( 3.0, 5.0 ) << "\n";

    return 0;
}
