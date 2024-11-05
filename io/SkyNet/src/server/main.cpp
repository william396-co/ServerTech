#include "utils/util.h"

#include "io/EventLoop.h"
#include "io/Server.h"

#include <iostream>

int main( int argc, char ** argv )
{
    if ( argc != 2 ) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    EventLoop * loop = new EventLoop();
    Server server( loop, argv[1] );
    loop->loop();

    return 0;
}

