#include "../../util/xtime.h"

#include <iostream>
#include <cstdlib>

int main( int argc, char ** argv )
{
    using namespace utils;

    auto now = time();
    int offset = 0;

    if ( argc > 1 ) {
        offset = atoi( argv[1] );
    }

    std::cout << getDateTime( now ) << "\n";

    auto t = now - OneHour * 11;
    std::cout << "[" << getDateTime( t ) << "]";
    auto zeroTime = getZeroTimeStamp( t, offset );
    if ( isOverDay( t, offset ) ) {
        std::cout << " is OverDay\t Zero Time is [" << getDateTime( zeroTime ) << "]\n ";
    } else {
        std::cout << " is Not OverDay\t Zero Time is [" << getDateTime( zeroTime ) << "]\n ";
    }

    return 0;
}
