#include "utils/csapp.h"
#include "utils/util.h"

#include <iostream>

int main( int argc, char ** argv )
{
    if ( argc != 2 ) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    auto listenfd = open_listenfd( argv[1] );
    errif( listenfd == -1, "socket accept error" );

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof( client_addr );

    auto fd = accept( listenfd, (sockaddr *)&client_addr, &addr_len );
    errif( fd == -1, "socket accept error" );

    std::cout << "new client fd " << fd << " IP: " << inet_ntoa( client_addr.sin_addr ) << " Port:" << ntohs( client_addr.sin_port ) << "\n";

    while ( true ) {
        char buf[1024];
        ssize_t read_bytes = read( fd, buf, sizeof( buf ) );
        if ( read_bytes > 0 ) {
            std::cout << "message from client fd:" << fd << "[" << buf << "]\n";
            write( fd, buf, sizeof( buf ) );
        } else if ( read_bytes == 0 ) {
            std::cout << "client fd: " << fd << " disconnected\n";
            close( fd );
            break;
        } else if ( read_bytes == -1 ) {
            close( fd );
            errif( true, "socket read error" );
        }
    }

    close( fd );
    return 0;
}
