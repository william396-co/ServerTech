#include "Socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include "util.h"

Socket::Socket()
    : fd( -1 )
{
    fd = socket( AF_INET, SOCK_STREAM, 0 );
    ErrIf( fd == -1, "socket create error" );
}

Socket::Socket( int fd )
    : fd { fd }
{
    ErrIf( this->fd == -1, "socket create error" );
}

Socket::~Socket()
{
    if ( fd != -1 ) {
        close( fd );
    }
}

void Socket::bind( InetAddress & addr )
{
    socklen_t addr_len = sizeof( addr.getAddr() );

    // set port reuseable
    int opt = 1;
    setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) );

    ErrIf( ::bind( fd, (sockaddr *)&addr.getAddr(), addr_len ) == -1, "socket bind error" );
}

void Socket::listen()
{
    ErrIf( ::listen( fd, SOMAXCONN ) == -1, "socket listen error" );
}

void Socket::setNonBlocking()
{
    fcntl( fd, F_SETFL, fcntl( fd, F_GETFL ) | O_NONBLOCK );
}
bool Socket::isNonBlocking()
{
    return fcntl( fd, F_GETFL ) & O_NONBLOCK != 0;
}

int Socket::accept( InetAddress & addr )
{
    socklen_t addr_len = sizeof( addr.getAddr() );

    int client_fd = -1;
    if ( fcntl( fd, F_GETFL ) & O_NONBLOCK ) {
        while ( true ) {
            client_fd = ::accept( fd, (sockaddr *)&addr, &addr_len );
            if ( client_fd == -1 && ( errno == EAGAIN || errno == EWOULDBLOCK ) ) {
                continue;
            }
            if ( client_fd == -1 ) {
                ErrIf( true, "socket accept error" );
            } else {
                break;
            }
        }
    } else {
        client_fd = ::accept( fd, (sockaddr *)&addr, &addr_len );
        ErrIf( client_fd == -1, "socket accept error" );
    }
    return client_fd;
}

void Socket::connect( InetAddress & addr )
{
    socklen_t addr_len = sizeof( addr.getAddr() );
    ErrIf( ::connect( fd, (sockaddr *)&addr.getAddr(), addr_len ) == -1, "socket connect error" );
}

void Socket::connect( const char * ip, uint16_t port )
{
    InetAddress addr { ip, port };
    connect( addr );
}

InetAddress::InetAddress() = default;

InetAddress::InetAddress( const char * ip, uint16_t port )
{
    bzero( &addr_, sizeof( addr_ ) );
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr( ip );
    addr_.sin_port = htons( port );
}

void InetAddress::setAddr( sockaddr_in addr )
{
    addr_ = addr;
}

sockaddr_in & InetAddress::getAddr()
{
    return addr_;
}

const char * InetAddress::getIp()
{
    return inet_ntoa( addr_.sin_addr );
}

uint16_t InetAddress::getPort()
{
    return ntohs( addr_.sin_port );
}

