#include <cstdlib>
#include <iostream>

int main()
{
    int values[] = { 8, 9, 2, 5, 1, 4, 7, 3, 6 };
    constexpr size_t numElements = sizeof( values ) / sizeof( values[0] );

    std::qsort( values, numElements, sizeof( int ), []( const void * a, const void * b ) noexcept {
        return ( *(int *)b - *(int *)a );
    } );

    for ( auto const & val : values ) {
        std::cout << val << ",";
    }
    std::cout << "\n";

    return 0;
}
