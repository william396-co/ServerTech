#include "utils/util.h"

#include "io/Epoll.h"
#include "io/Socket.h"

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

using SocketMap = std::unordered_map<int, Socket *>;
SocketMap socketlist;

void addSocket( Socket * s )
{
    socketlist.emplace( s->getFd(), s );
}

void delSocket( int fd )
{
    auto it = socketlist.find( fd );
    if ( it != socketlist.end() ) {
        delete it->second;
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
    std::unique_ptr<Epoll> ep = std::make_unique<Epoll>();
    errif( ep->getFd() == -1, "Epoll create error" );

    listenSocket->setnonblocking();
    ep->addFd( listenSocket->getFd(), EPOLLIN | EPOLLET );

    while ( true ) {

        auto events = ep->poll();
        for ( size_t i = 0; i != events.size(); ++i ) {
            if ( events[i].data.fd == listenSocket->getFd() ) {

                sockaddr_in addr {};
                Socket * clientSocket = new Socket( listenSocket->accept( addr ) );
                clientSocket->setRemote( addr );
                std::cout << "new client fd:" << clientSocket->getFd() << " Ip:" << clientSocket->remoteIp() << " Port:" << clientSocket->remotePort() << "\n";
                clientSocket->setnonblocking();
                ep->addFd( clientSocket->getFd(), EPOLLIN | EPOLLET );
                addSocket( clientSocket );
            } else if ( events[i].events & EPOLLIN ) {
                handleReadEvent( events[i].data.fd );
            } else {
                std::cout << "something else happend!\n";
            }
        }
    }

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
