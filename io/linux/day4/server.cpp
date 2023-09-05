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
#include "Channel.h"
#include "InetAddress.h"

constexpr auto READ_BUFFER = 1024;

void handleReadEvent( int fd );

using SocketMap = std::unordered_map<int, Socket *>;
SocketMap socketList;
using ChannelMap = std::unordered_map<int, Channel *>;
ChannelMap channelList;

void addSocket( Socket * s, Channel * ch )
{
    socketList.emplace( s->getFd(), s );
    channelList.emplace( ch->getFd(), ch );
}
void delSocket( int fd )
{
    auto it = socketList.find( fd );
    if ( it != socketList.end() ) {
        delete it->second;
        socketList.erase( it );
    }

    auto it2 = channelList.find( fd );
    if ( it2 != channelList.end() ) {
        delete it2->second;
        channelList.erase( it2 );
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
    Channel * sCh = new Channel( ep, listenSock->getFd() );
    sCh->enableReading();

    while ( true ) {
        auto activeChannels = ep->poll();
        for ( size_t i = 0; i != activeChannels.size(); ++i ) {
            if ( activeChannels[i]->getFd() == listenSock->getFd() ) {
                InetAddress c_addr {};
                Socket * clientSock = new Socket( listenSock->accept( c_addr ) );
                printf( "new client fd %d! IP: %s Port: %d\n", clientSock->getFd(), inet_ntoa( c_addr.addr.sin_addr ), ntohs( c_addr.addr.sin_port ) );
                clientSock->setnonblocking();
                Channel * cCh = new Channel( ep, clientSock->getFd() );
                cCh->enableReading();
                addSocket( clientSock, cCh );
            } else if ( activeChannels[i]->getEvents() & EPOLLIN ) {
                handleReadEvent( activeChannels[i]->getFd() );
            } else {
                printf( "something else happened\n" );
            }
        }
    }

    delete ep;
    delete listenSock;
    delete sCh;
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
