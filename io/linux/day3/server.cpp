#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <unordered_map>
#include "util.h"
#include "Epoll.h"
#include "Socket.h"
#include "InetAddress.h"

constexpr auto READ_BUFFER = 1024;

void handleReadEvent( int fd );

using SocketMap = std::unordered_map<int, Socket *>;
SocketMap socketList;

void addSocket( Socket * s )
{
    socketList.emplace( s->getFd(), s );
}
void delSocket( int fd )
{
    auto it = socketList.find( fd );
    if ( it != socketList.end() ) {
        delete it->second;
        socketList.erase( it );
    }
}

int main()
{
    Socket * listenSock = new Socket();
    InetAddress s_addr( "127.0.0.1", 9527 );
    listenSock->bind( s_addr );
    listenSock->listen();

    Epoll * ep = new Epoll();
    listenSock->setnonblocking();
    ep->addFd( listenSock->getFd(), EPOLLIN | EPOLLET );

    while ( true ) {
        auto events = ep->poll();
        for ( size_t i = 0; i != events.size(); ++i ) {
            if ( events[i].data.fd == listenSock->getFd() ) {
                InetAddress c_addr {};
                Socket * clientSock = new Socket( listenSock->accept( c_addr ) );
                printf( "new client fd %d! IP: %s Port: %d\n", clientSock->getFd(), inet_ntoa( c_addr.addr.sin_addr ), ntohs( c_addr.addr.sin_port ) );
                clientSock->setnonblocking();
                ep->addFd( clientSock->getFd(), EPOLLIN | EPOLLET );
                addSocket( clientSock );
            } else if ( events[i].events & EPOLLIN ) {
                handleReadEvent( events[i].data.fd );
            } else {
                printf( "something else happened\n" );
            }
        }
    }

    delete ep;
    delete listenSock;
}

void handleReadEvent( int fd )
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
            delSocket( fd );
            break;
        }
    }
}
