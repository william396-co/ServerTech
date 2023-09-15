#include "Connection.h"

#include "Buffer.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include <cassert>

constexpr auto READ_BUFFER = 1024;

Connection::Connection( EventLoop * loop, Socket * s )
    : loop_ { loop }, sock_ { s }, channel_ { nullptr }
{
    if ( loop_ ) {
        channel = new Channel( loop, s->getFd() );
        channel->enableRead();
        channel->useET();
    }
    read_buffer_ = new Buffer();
    send_buffer_ = new Buffer();
    state_ = State::Connected;
}

Connection::~Connection()
{
    if ( channel_ ) {
        delete channel_;
    }
    delete writeBuffer_;
    delete sendBuffer_;
    delete sock_;
    delete channel_;
}

void Connection::setSendBuffer( const char * data )
{
    send_buffer_->setBuf( data );
}
const char * Connection::getReadBuffer()
{
    return read_buffer_->c_str();
}
const char * Connection::getSendBuffer()
{
    return send_buffer_->c_str();
}

void Connection::getLineSendBuffer()
{
    return send_buffer_->getline();
}

void Connection::setOnConnectCallback( ConnectionCallback const & cb )
{
    on_connect_callback_ = cb;
    channel_->setReadCallback( [this] { on_connect_callback_( this ); } );
}

void Connection::read()
{
    assert( state_ == State::Connected, "connection state is disconnected!" );
    read_buffer->clear();
    if ( sock_->isNonBlocking() ) {
        readNonblocking();
    } else {
        readBlocking();
    }
}

void Connection::write()
{
    assert( state_ == State::Connected, "connection state is disconnected!" );
    if ( sock_->isNonBlocking() ) {
        writeNonBlocking();
    } else {
        writeBlocking();
    }
    send_buffer_->clear();
}

void Connection::close()
{
    delete_connect_callback_( sock_ );
}

void Connection::readNonBlocking()
{
}

void Connection::writeNonBlocking()
{
}

/* only use by client */
void Connection::readBlocking()
{
    int fd = sock_->getFd();
    unsigned recv_size = 0;
    socklen_t len = sizeof( recv_size );
    getsockopt( fd, SOL_SOCKET, SO_RECVBUF, &recv_size, &len );
    char buf[recv_size];
    ssize_t bytes_read = read( fd, buf, sizeof( buf ) );
    if ( bytes_read > 0 ) {
        read_buffer_->append( buf, bytes_read );
    } else if ( bytes_read == 0 ) {
        printf( "read EOF, blocking client fd %d disconnected\n", fd );
        state_ = State::Closed;
    } else if ( bytes_read == -1 ) {
        printf( "Other error no blocking client fd %d\n", fd );
        state_ = State::Closed;
    }
}

void Connection::writeBlocking()
{
    int fd = sock_->getFd();
    ssize_t bytes_write = write( fd, send_buffer_->c_str(), send_buffer_->size() );
    if ( bytes_write == -1 ) {
        printf( "Other error on blocking client fd %d\n", fd );
        state_ = State::Closed;
    }
}
