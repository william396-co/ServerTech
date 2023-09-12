#include "Socket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include "util.h"
#include "InetAddress.h"

Socket::Socket()
    : fd( -1 )
{
    fd = socket( AF_INET, SOCK_STREAM, 0 );
    errif( fd == -1, "socket create error" );
}

Socket::Socket( int fd )
    : fd { fd }
{
    errif( this->fd == -1, "socket create error" );
}

Socket::~Socket()
{
    if ( fd != -1 ) {
        close( fd );
    }
}

void Socket::bind( InetAddress & _addr )
{
    struct sockaddr_in addr = _addr.getAddr();
    socklen_t addr_len = _addr.getAddr_len();

    // set port reuseable
    int opt = 1;
    setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) );

    errif( ::bind( fd, (sockaddr *)&addr, addr_len ) == -1, "socket bind error" );
    _addr.setInetAddr( addr, addr_len );
}

void Socket::listen()
{
    errif( ::listen( fd, SOMAXCONN ) == -1, "socket listen error" );
}

void Socket::setnonblocking()
{
    fcntl( fd, F_SETFL, fcntl( fd, F_GETFL ) | O_NONBLOCK );
}

int Socket::accept( InetAddress & _addr )
{
    struct sockaddr_in addr;
    bzero( &addr, sizeof( addr ) );
    socklen_t addr_len = sizeof( addr );

    int client_fd = ::accept( fd, (sockaddr *)&addr, &addr_len );
    errif( client_fd == -1, "socket accept error" );
    _addr.setInetAddr( addr, addr_len );
    return client_fd;
}

void Socket::connect( InetAddress & _addr )
{
    struct sockaddr_in addr = _addr.getAddr();
    socklen_t addr_len = _addr.getAddr_len();
    errif( ::connect( fd, (sockaddr *)&addr, addr_len ) == -1, "socket connect error" );
}


InetAddress::InetAddress()
    : addr_len( sizeof( addr ) )
{
    bzero( &addr, sizeof( addr ) );
}

InetAddress::InetAddress( const char * ip, uint16_t port )
    : addr_len( sizeof( addr ) )
{
    bzero( &addr, sizeof( addr ) );
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr( ip );
    addr.sin_port = htons( port );
}

void InetAddress::setInetAddr( sockaddr_in _addr, socklen_t _addr_len )
{
    addr = _addr;
    addr_len = _addr_len;
}

