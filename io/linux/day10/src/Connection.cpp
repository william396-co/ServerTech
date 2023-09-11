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
    channel->enableRead();
    channel->useET();
    ChannelReadCallback cb = std::bind( &Connection::echo, this, s->getFd() );
    channel->setReadCallback( cb );
    channel->setUseThreadPool();

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
            // errif( write( fd, readBuffer->c_str(), readBuffer->size() ) == -1, "socket write error" );
            send( fd );
            readBuffer->clear();
            break;
        } else if ( bytes_read == 0 ) { // EOF, disconnect
            printf( "EOF, client fd %d disconnect\n", fd );
            deleteCallback( fd );
            break;
        } else {
            printf( "Connection reset by perr\n" );
            deleteCallback( fd );
        }
    }
}

void Connection::send( int fd )
{
    char buf[readBuffer->size()];
    strcpy( buf, readBuffer->c_str() );
    int data_size = readBuffer->size();
    int data_left = data_size;
    while ( data_left > 0 ) {
        ssize_t bytes_write = write( fd, buf + data_size - data_left, data_left );
        if ( bytes_write == -1 && errno == EAGAIN ) {
            break;
        }
        data_left -= bytes_write;
    }
}
