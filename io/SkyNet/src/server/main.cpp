#include "utils/util.h"

#include "io/Epoll.h"
#include "io/Socket.h"
#include "io/Channel.h"

#include <iostream>
#include <sys/epoll.h>
#include <errno.h>
#include <cstring>
#include <memory>
#include <unistd.h>
#include <unordered_map>

constexpr auto MAX_EVENTS = 1024;
constexpr auto READ_BUFFER = 1024;

void handleReadEvent( int fd );

using SocketChannelMap = std::unordered_map<int, std::pair<Socket *, Channel *>>;
SocketChannelMap socketlist;

void addSocket( Socket * s, Channel * ch )
{
    socketlist.emplace( s->getFd(), std::pair { s, ch } );
}

void delSocket( int fd )
{
    auto it = socketlist.find( fd );
    if ( it != socketlist.end() ) {
        delete it->second.first;
        delete it->second.second;
        socketlist.erase( it );
    }
}

int main( int argc, char ** argv )
{
    if ( argc != 2 ) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    // create listenSocket
    std::unique_ptr<Socket> listenSocket = std::make_unique<Socket>();
    listenSocket->listen( argv[1] );
    errif( listenSocket->getFd() == -1, "socket listen error" );

    // create Epoll
    Epoll * ep = new Epoll();
    errif( ep->getFd() == -1, "Epoll create error" );

    listenSocket->setnonblocking();
    std::unique_ptr<Channel> sCh = std::make_unique<Channel>( ep, listenSocket->getFd() );
    sCh->enableReading();

    while ( true ) {

        auto channels = ep->poll();
        for ( size_t i = 0; i != channels.size(); ++i ) {
            auto chfd = channels[i]->getFd();
            if ( chfd == listenSocket->getFd() ) {

                sockaddr_in addr {};
                Socket * clientSocket = new Socket( listenSocket->accept( addr ) );
                clientSocket->setRemote( addr );
                std::cout << "new client fd:" << clientSocket->getFd() << " Ip:" << clientSocket->remoteIp() << " Port:" << clientSocket->remotePort() << "\n";
                clientSocket->setnonblocking();
                Channel * ch = new Channel( ep, clientSocket->getFd() );
                ch->enableReading();
                addSocket( clientSocket, ch );
            } else if ( channels[i]->getRevents() & EPOLLIN ) {
                handleReadEvent( chfd );
            } else {
                std::cout << "something else happend!\n";
            }
        }
    }
    delete ep;
    return 0;
}

void handleReadEvent( int fd )
{
    char buf[1024];
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
            delSocket( fd );
            break;
        }
    }
}
