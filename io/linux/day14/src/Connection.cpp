#include "Connection.h"

#include "Buffer.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <utility>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr auto READ_BUFFER = 1024;

Connection::Connection( EventLoop * loop, Socket * s )
    : loop_ { loop }, sock_ { s }, channel_ { nullptr }
{
    if ( loop_ ) {
        channel_ = new Channel( loop, s );
        channel_->enableRead();
        channel_->useET();
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
    delete read_buffer_;
    delete send_buffer_;
    delete sock_;
    delete channel_;
}

void Connection::setSendBuffer( const char * data )
{
    send_buffer_->setBuf( data );
}
const char * Connection::readBuffer()
{
    return read_buffer_->c_str();
}
const char * Connection::sendBuffer()
{
    return send_buffer_->c_str();
}

void Connection::getLineSendBuffer()
{
    return send_buffer_->getline();
}

void Connection::setOnConnectCallback( ConnectCallback const & cb )
{
    on_connect_callback_ = cb;
    channel_->setReadCallback( [this] { on_connect_callback_( this ); } );
}

void Connection::read()
{
    ASSERT( state_ == State::Connected, "connection state is disconnected!" );
    read_buffer_->clear();
    if ( sock_->isNonBlocking() ) {
        readNonBlocking();
    } else {
        readBlocking();
    }
}

void Connection::write()
{
    ASSERT( state_ == State::Connected, "connection state is disconnected!" );
    if ( sock_->isNonBlocking() ) {
        writeNonBlocking();
    } else {
        writeBlocking();
    }
    send_buffer_->clear();
}

void Connection::close()
{
    delete_connection_callback_( sock_->getFd() );
}

void Connection::readNonBlocking()
{
    int fd = sock_->getFd();
    char buf[1024];
    while ( true ) {
        bzero( buf, sizeof( buf ) );
        ssize_t bytes_read = ::read( fd, buf, sizeof( buf ) );
        if ( bytes_read > 0 ) {
        } else if ( bytes_read == -1 && errno == EINTR ) {
            printf( "continuing reading\n" );
            continue;
        } else if ( bytes_read == -1 && ( ( errno == EAGAIN ) || ( errno = EWOULDBLOCK ) ) ) { // NonBloc IO
            break;
        } else if ( bytes_read == 0 ) { // EOF,client disconnected
            printf( "read EOF, client fd: %d disconnected\n", fd );
            state_ = State::Closed;
            break;
        } else {
            printf( "Other error on client fd: %d\n", fd );
            state_ = State::Closed;
            break;
        }
    }
}

void Connection::writeNonBlocking()
{
    int fd = sock_->getFd();
    char buf[send_buffer_->size()];
    size_t data_size = send_buffer_->size();
    memcpy( buf, send_buffer_->c_str(), data_size );

    size_t data_left = data_size;
    while ( data_left > 0 ) {
        ssize_t bytes_write = ::write( fd, buf + data_size - data_left, data_left );
        if ( bytes_write == -1 && errno == EINTR ) {
            printf( "continue writing\n" );
            continue;
        }
        if ( bytes_write == -1 && errno == EAGAIN ) {
            break;
        }
        if ( bytes_write == -1 ) {
            printf( "Other error on client fd:%d\n", fd );
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
}

/* only use by client */
void Connection::readBlocking()
{
    int fd = sock_->getFd();
    unsigned recv_size = 0;
    socklen_t len = sizeof( recv_size );
    getsockopt( fd, SOL_SOCKET, SO_RCVBUF, &recv_size, &len );
    char buf[recv_size];
    ssize_t bytes_read = ::read( fd, buf, sizeof( buf ) );
    if ( bytes_read > 0 ) {
        read_buffer_->append( buf, bytes_read );
    } else if ( bytes_read == 0 ) {
        printf( "read EOF, blocking client fd: %d disconnected\n", fd );
        state_ = State::Closed;
    } else if ( bytes_read == -1 ) {
        printf( "Other error no blocking client fd: %d\n", fd );
        state_ = State::Closed;
    }
}

void Connection::writeBlocking()
{
    int fd = sock_->getFd();
    ssize_t bytes_write = ::write( fd, send_buffer_->c_str(), send_buffer_->size() );
    if ( bytes_write == -1 ) {
        printf( "Other error on blocking client fd: %d\n", fd );
        state_ = State::Closed;
    }
}
