#include "Server.h"

#include "Socket.h"
#include "Channel.h"

#include <functional>
#include <cstring>
#include <unistd.h>
#include <iostream>

Server::Server( EventLoop * loop, char * port )
    : loop_ { loop }
{

    Socket * listenSocket = new Socket();
    listenSocket->listen( port );
    listenSocket->setnonblocking();

    Channel * sCh = new Channel( loop, listenSocket->getFd() );
    std::function<void()> cb = std::bind( &Server::newConnection, this, listenSocket );
    sCh->setCallback( cb );
    sCh->enableReading();
}

Server::~Server()
{
    clear();
}

constexpr auto READ_BUFFER = 1024;
void Server::handleReadEvent( int fd )
{
    char buf[READ_BUFFER];
    while ( true ) { // because use io nonblocking,
        ssize_t read_bytes = read( fd, buf, sizeof( buf ) );
        if ( read_bytes > 0 ) {
            std::cout << "message from client fd:" << fd << "[" << buf << "]\n";
            write( fd, buf, sizeof( buf ) );
        } else if ( read_bytes == -1 && errno == EINTR ) {
            std::cout << "continue reading\n";
            continue;
        } else if ( read_bytes == -1 && ( errno == EAGAIN || errno == EWOULDBLOCK ) ) {
            std::cout << "finish reading once, errno: " << errno << "\n";
            break;
        } else if ( read_bytes == 0 ) {
            std::cout << "client fd: " << fd << " disconnected\n";
            delSocketChannel( fd );
            break;
        }
    }
}

void Server::newConnection( Socket * listenSocket )
{
    sockaddr_in addr {};
    Socket * clientSocket = new Socket( listenSocket->accept( addr ) );
    clientSocket->setRemote( addr );
    std::cout << "new client fd:" << clientSocket->getFd() << " Ip:" << clientSocket->remoteIp() << " Port:" << clientSocket->remotePort() << "\n";
    clientSocket->setnonblocking();

    Channel * ch = new Channel( loop_, clientSocket->getFd() );
    std::function<void()> cb = std::bind( &Server::handleReadEvent, this, clientSocket->getFd() );
    ch->setCallback( cb );
    ch->enableReading();
    addSocketChannel( clientSocket->getFd(), clientSocket, ch );
}

void Server::addSocketChannel( int fd, Socket * s, Channel * ch )
{

    socketList_.emplace( fd, std::pair { s, ch } );
}

void Server::delSocketChannel( int fd )
{
    auto it = socketList_.find( fd );
    if ( it != socketList_.end() ) {
        delete it->second.first;
        delete it->second.second;
        socketList_.erase( it );
    }
}

void Server::clear()
{
    for ( auto & it : socketList_ ) {
        delete it.second.first;
        delete it.second.second;
    }
}
