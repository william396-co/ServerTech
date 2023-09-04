#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "util.h"
#include "Epoll.h"
#include "Socket.h"
#include "InetAddress.h"

constexpr auto READ_BUFFER = 1024;
constexpr auto ip = "0.0.0.1";
constexpr auto port = 9527;

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
    std::unique_ptr<Socket> listenSock = std::make_unique<Socket>();
    std::unique_ptr<InetAddress> s_addr = std::make_unique<InetAddress>( ip, port );
    listenSock->bind( s_addr.get() );

    listenSock->listen();

    std::unique_ptr<Epoll> ep = std::make_unique<Epoll>();
    listenSock->setnonblocking();
    ep->addFd( listenSock->getFd(), EPOLLIN | EPOLLET );

    while ( true ) {
        auto events = ep->poll();
        for ( size_t i = 0; i != events.size(); ++i ) {
            if ( events[i].data.fd == listenSock->getFd() ) {
                std::unique_ptr<InetAddress> c_addr = std::make_unique<InetAddress>();
                Socket * clientSock = new Socket( listenSock->accept( c_addr.get() ) ); // memory leak
                printf( "new client fd %d! IP: %s Port: %d\n", clientSock->getFd(), inet_ntoa( c_addr->addr.sin_addr ), ntohs( c_addr->addr.sin_port ) );
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
