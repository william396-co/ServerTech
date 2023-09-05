#include "Server.h"

#include "InetAddress.h"
#include "Socket.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include "util.h"
#include <functional>
#include <cstring>
#include <unistd.h>
#include <cassert>

constexpr auto READ_BUFFER = 1024;

Server::Server( EventLoop * _loop, const char * ip, uint16_t port )
    : loop { _loop }
{
    acceptor = new Acceptor { _loop, ip, port };

    // assert( loop != nullptr );
    // assert( acceptor != nullptr );
    ConnectionCallback cb = std::bind( &Server::newConnection, this, std::placeholders::_1 );
    acceptor->setNewConnectionCallback( cb );
}

Server::~Server()
{
    for ( auto & it : connections ) {
        delete it.second;
    }

    delete acceptor;
}

void Server::deleteConnection( Socket * _s )
{
    auto it = connections.find( _s->getFd() );
    if ( it != connections.end() ) {
        delete it->second;
        connections.erase( it );
    }
}

void Server::newConnection( Socket * _s )
{
    Connection * conn = new Connection( loop, _s );
    ConnectionCallback cb = std::bind( &Server::deleteConnection, this, std::placeholders::_1 );
    conn->setDeleteConnectionCallback( cb );
    connections.emplace( _s->getFd(), conn );
}
