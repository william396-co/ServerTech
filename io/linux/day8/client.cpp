#include <cstring>
#include <memory>
#include <unistd.h>

#include "src/util.h"
#include "src/Socket.h"
#include "src/InetAddress.h"
#include "src/Buffer.h"

constexpr auto BUFFER_SIZE = 1024;
constexpr auto ip = "127.0.0.1";
constexpr auto port = 9527;

int main()
{

    std::unique_ptr<Socket> s = std::make_unique<Socket>();
    InetAddress addr { ip, port };
    s->connect( addr );

    int fd = s->getFd();

    std::unique_ptr<Buffer> sendBuffer = std::make_unique<Buffer>();
    std::unique_ptr<Buffer> readBuffer = std::make_unique<Buffer>();

    while ( true ) {
        sendBuffer->getline();
        ssize_t write_bytes = write( fd, sendBuffer->c_str(), sendBuffer->size() );
        if ( write_bytes == -1 ) {
            printf( "socket already disconnected, can't write any more!\n" );
            break;
        }

        while ( true ) {
            sendBuffer->getline();
            ssize_t write_bytes = write( fd, sendBuffer->c_str(), sendBuffer->size() );
            if ( write_bytes == -1 ) {
                printf( "socket already disconnected, can't write any more!\n" );
                break;
            }

            ssize_t already_read = 0;
            char buf[BUFFER_SIZE];
            bzero( &buf, sizeof( buf ) );
            ssize_t read_bytes = read( fd, buf, sizeof( buf ) );
            if ( read_bytes > 0 ) {
                readBuffer->append( buf, read_bytes );
                already_read += read_bytes;

                printf( "message from server: %s\n", buf );
            } else if ( read_bytes == 0 ) { // EOF
                printf( "server socket disconnected!\n" );
                break;
            } else if ( already_read >= sendBuffer->size() ) {
                printf( "message from server: %s\n", readBuffer->c_str() );
                break;
            }
        }
        readBuffer->clear();
    }
    return 0;
}
