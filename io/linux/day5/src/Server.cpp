#include "Server.h"

#include "InetAddress.h"
#include "Socket.h"
#include "EventLoop.h"
#include "Channel.h"
#include <functional>
#include <cstring>
#include <unistd.h>

constexpr auto READ_BUFFER = 1024;

Server::Server( EventLoop * loop, const char * ip, uint16_t port )
    : loop { loop }
{
    Socket * listenfd = new Socket();
    InetAddress sAddr { ip, port };
    listenfd->bind( sAddr );
    listenfd->listen();
    listenfd->setnonblocking();

    Channel * listenCh = new Channel { loop, listenfd->getFd() };
    std::function<void()> cb = std::bind( &Server::newConnection, this, listenfd );
    listenCh->setCallback( cb );
    listenCh->enableReading();
    addSocketChannel( listenfd->getFd(), listenfd, listenCh );
}

void Server::addSocketChannel( int fd, Socket * s, Channel * ch )
{
    socketList.emplace( fd, s );
    channelList.emplace( fd, ch );
}

void Server::delSocketChannel( int fd )
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

void Server::clear()
{
    for ( auto & it : socketList ) {
        delete it.second;
    }
    for ( auto & it : channelList ) {
        delete it.second;
    }
}

void Server::handleReadEvent( int fd )
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
            delSocketChannel( fd );
            break;
        }
    }
}

void Server::newConnection( Socket * listenfd )
{
    InetAddress cAddr {};
    Socket * s = new Socket( listenfd->accept( cAddr ) );
    printf( "new client fd %d! Ip: %s Port: %d\n", s->getFd(), inet_ntoa( cAddr.addr.sin_addr ), ntohs( cAddr.addr.sin_port ) );
    s->setnonblocking();

    Channel * ch = new Channel( loop, s->getFd() );
    ChannelCallback cb = std::bind( &Server::handleReadEvent, this, s->getFd() );
    ch->setCallback( cb );
    ch->enableReading();
    addSocketChannel( s->getFd(), s, ch );
}
