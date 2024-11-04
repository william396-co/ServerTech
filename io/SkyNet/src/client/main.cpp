#include "utils/util.h"
#include "io/Socket.h"

#include <iostream>
#include <memory>
#include <unistd.h>
#include <cstring>

int main( int argc, char ** argv )
{

    if ( argc != 3 ) {
        std::cout << argv[0] << " Usage<ip><port>\n";
        return 1;
    }

    std::unique_ptr<Socket> clientSocket = std::make_unique<Socket>();
    clientSocket->connect( argv[1], argv[2] );
    errif( clientSocket->getFd() == -1, "socket connect error" );

    char buf[1024];
    while ( true ) {

        auto clientfd = clientSocket->getFd();
        scanf( "%s", buf );

        ssize_t write_bytes = write( clientfd, buf, sizeof( buf ) );
        if ( write_bytes == -1 ) {
            std::cout << "socket already disconnected,can't write any more!\n";
            break;
        }
        memset( buf, '0', sizeof( buf ) );
        ssize_t read_bytes = read( clientfd, buf, sizeof( buf ) );
        if ( read_bytes ) {
            std::cout << "message from server:" << buf << "\n";
        } else if ( read_bytes == 0 ) {
            std::cout << "server socketd disconnected!\n";
            break;
        } else if ( read_bytes == -1 ) {
            errif( true, "socket read error" );
        }
    }
    return 0;
}
