#include "Connection.h"

#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Buffer.h"

#include <unistd.h>
#include <iostream>

Connection::Connection( EventLoop * loop, Socket * s )
    : loop_ { loop }, s_ { s }
{
    ch_ = new Channel( loop_, s->getFd() );

    std::function<void()> cb = std::bind( &Connection::echo, this, s_->getFd() );
    ch_->setCallback( cb );
    ch_->enableReading();

    readBuffer_ = new Buffer();
    writeBuffer_ = new Buffer();
}

Connection::~Connection()
{
    delete ch_;
    delete s_;

    delete readBuffer_;
    delete writeBuffer_;
}

void Connection::echo( int fd )
{
    constexpr auto READ_BUFFER = 1024;
    char buf[READ_BUFFER];
    while ( true ) { // because use io nonblocking,
        ssize_t read_bytes = read( fd, buf, sizeof( buf ) );
        if ( read_bytes > 0 ) {
            readBuffer_->append( buf, read_bytes );
            // std::cout << "message from client fd:" << fd << "[" << buf << "]\n";
            write( fd, buf, sizeof( buf ) );
        } else if ( read_bytes == -1 && errno == EINTR ) {
            std::cout << "continue reading\n";
            continue;
        } else if ( read_bytes == -1 && ( errno == EAGAIN || errno == EWOULDBLOCK ) ) {
            std::cout << "finish reading once\n";
            std::cout << "message from client fd:" << fd << " [" << readBuffer_->c_str() << "]\n";
            readBuffer_->clear();
            break;
        } else if ( read_bytes == 0 ) {
            std::cout << "client fd: " << fd << " disconnected\n";
            if ( deleteConnectionCallback_ ) {
                deleteConnectionCallback_( s_ );
            }
            break;
        }
    }
}

