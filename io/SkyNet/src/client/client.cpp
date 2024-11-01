#include "utils/xtime.h"
#include "utils/csapp.h"

#include <iostream>

int main( int argc, char ** argv )
{

    if ( argc != 3 ) {
        std::cout << argv[0] << " Usage<ip><port>\n";
        return 1;
    }

    std::cout << "2012 is a leapYear: " << std::boolalpha << utils::isLeapYear( 2024 ) << "\n";

    int clientfd = open_clientfd( argv[1], argv[2] );

    close( clientfd );
    return 0;
}
