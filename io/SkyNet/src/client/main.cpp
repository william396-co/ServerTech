#include "utils/util.h"
#include "io/Socket.h"
#include "utils/Buffer.h"

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

    std::unique_ptr<Buffer> sendBuffer = std::make_unique<Buffer>();
    std::unique_ptr<Buffer> readBuffer = std::make_unique<Buffer>();
    auto clientfd = clientSocket->getFd();

    while ( true ) {

        sendBuffer->getline();
        ssize_t write_bytes = write( clientfd, sendBuffer->c_str(), sendBuffer->size() );
        if ( write_bytes == -1 ) {
            std::cout << "socket already disconnected,can't write any more!\n";
            break;
        }

        size_t already_read = 0;
        char buf[1024];
        while ( true ) {
            ssize_t read_bytes = read( clientfd, buf, sizeof( buf ) );
            if ( read_bytes ) {
                readBuffer->append( buf, read_bytes );
                already_read += read_bytes;
                //                std::cout << "message from server:" << buf << "\n";
            } else if ( read_bytes == 0 ) {
                std::cout << "server socketd disconnected!\n";
                exit( EXIT_FAILURE );
            } else if ( read_bytes == -1 ) {
                errif( true, "socket read error" );
            }

            if ( already_read >= sendBuffer->size() ) {
                std::cout << "message from server: [" << readBuffer->c_str() << "]\n";
                break;
            }
        }
        readBuffer->clear();
    }

    return 0;
}
