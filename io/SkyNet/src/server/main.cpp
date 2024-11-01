#include "utils/csapp.h"

#include <iostream>

int main( int argc, char ** argv )
{
    if ( argc != 2 ) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    auto listenfd = open_listenfd( argv[1] );
    while ( true ) {

        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof( client_addr );

        auto fd = accept( listenfd, (sockaddr *)&client_addr, &addr_len );

        std::cout << "new client fd " << fd << " IP: " << inet_ntoa( client_addr.sin_addr ) << " Port:" << ntohs( client_addr.sin_port ) << "\n";
    }

    return 0;
}
