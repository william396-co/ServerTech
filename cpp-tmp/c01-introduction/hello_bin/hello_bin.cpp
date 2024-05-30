#include <iostream>

#define USE_META
#include "binary.hpp"

int main()
{

#ifdef USE_META
    std::cout << binary<101010>::value << "\n";

#else
    std::cout << binary( 101010 ) << "\n";
#endif

    return 0;
}
