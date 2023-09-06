#include "Connection.h"

#include "Buffer.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>

constexpr auto READ_BUFFER = 1024;

Connection::Connection( EventLoop * _loop, Socket * _s )
    : loop { _loop }, s { _s }, channel { nullptr }
{
    channel = new Channel( loop, s->getFd() );
    ChannelCallback cb = std::bind( &Connection::echo, this, s->getFd() );
    channel->setCallback( cb );
    channel->enableReading();

    readBuffer = new Buffer();
    writeBuffer = new Buffer();
}

Connection::~Connection()
{
    delete writeBuffer;
    delete readBuffer;
    delete s;
    delete channel;
}

void Connection::echo( int fd )
{
    char buf[READ_BUFFER];
    while ( true ) {
        bzero( &buf, sizeof( buf ) );
        ssize_t bytes_read = read( fd, buf, sizeof( buf ) );
        if ( bytes_read > 0 ) {
            printf( "message from client fd: %d: %s\n", fd, buf );
            readBuffer->append( buf, bytes_read );
        } else if ( bytes_read == -1 && errno == EINTR ) {
            printf( "continue reading" );
            continue;
        } else if ( bytes_read == -1 && ( ( errno == EAGAIN || ( errno == EWOULDBLOCK ) ) ) ) {
            printf( "finish reading once, errno:%d\n", errno );
            printf( "message from client fd: %d: %s\n", fd, readBuffer->c_str() );
            errif( write( fd, readBuffer->c_str(), readBuffer->size() ) == -1, "socket write error" );
            readBuffer->clear();
            break;
        } else if ( bytes_read == 0 ) { // EOF, disconnect
            printf( "EOF, client fd %d disconnect\n", fd );
            deleteCallback( s );
            break;
        }
    }
}
