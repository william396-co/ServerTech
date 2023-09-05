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

