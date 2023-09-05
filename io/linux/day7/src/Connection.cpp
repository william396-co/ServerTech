#include "Connection.h"

#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>

constexpr auto READ_BUFFER = 1024;

Connection::Connection( EventLoop * _loop, Socket * _s )
    : loop { _loop }, s { nullptr }, channel { nullptr }
{
    s = std::make_unique<Socket>( s );
    channel = std::make_unique<Channel>( loop, s->getFd() );
    ConnectionCallback cb = std::bind( &Connection::echo, this, s->getFd() );
    channel->setCallback( cb );
    channel->enableReading();
}

void Connection::echo( int fd )
{
    char buf[READ_BUFFER];
    while ( true ) {

        bzero( &buf, sizeof( buf ) );
        ssize_t bytes_read = read( fd, buf, sizeof( buf ) );
        if ( bytes_read > 0 ) {
            printf( "message from client fd: %d: %s\n", fd, buf );
            write( fd, buf, sizeof( buf ) );
        } else if ( bytes_read == -1 && errno == EINTR ) {
            printf( "continue reading" );
            continue;
        } else if ( bytes_read == -1 && ( ( errno == EAGAIN || ( errno == EWOULDBLOCK ) ) ) ) {
            printf( "finish reading once, errno:%d\n", errno );
            break;
        } else if ( bytes_read == 0 ) { // EOF, disconnect
            printf( "EOF, client fd %d disconnect\n", fd );
            deleteConnectionCallback( s );
            break;
        }
    }
}
