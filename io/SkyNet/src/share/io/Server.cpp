#include "Server.h"

#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"

#include <functional>
#include <cstring>
#include <unistd.h>
#include <iostream>

Server::Server( EventLoop * loop, char * port )
    : loop_ { loop }
{
    acceptor_ = new Acceptor( loop, port );
    NewConnCallback cb = std::bind( &Server::newConnection, this, std::placeholders::_1 );
    acceptor_->setNewConnectionCallback( cb );
}

Server::~Server()
{
    delete acceptor_;
}

void Server::newConnection( Socket * s )
{
    Connection * conn = new Connection( loop_, s );
    DeleteConnectionCallback cb = std::bind( &Server::deleteConnection, this, std::placeholders::_1 );
    conn->setDeleteConnectionCallback( cb );
    connections_[s->getFd()] = conn;
}

void Server::deleteConnection( Socket * s )
{
    auto it = connections_.find( s->getFd() );
    if ( it != connections_.end() ) {
        delete it->second;
        connections_.erase( it );
    }
}

