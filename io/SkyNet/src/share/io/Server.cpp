#include "Server.h"

#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "utils/ThreadPool.h"

#include <functional>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <thread>

Server::Server( EventLoop * loop, char * port )
    : mainReactor_ { loop }
{
    acceptor_ = new Acceptor( loop, port );
    NewConnCallback cb = std::bind( &Server::newConnection, this, std::placeholders::_1 );
    acceptor_->setNewConnectionCallback( cb );

    auto size = std::thread::hardware_concurrency();
    thpool_ = new ThreadPool( size );
    for ( int i = 0; i != size; ++i ) {
        subReactors_.emplace_back( new EventLoop() );
    }

    for ( int i = 0; i != size; ++i ) {
        std::function<void()> sub_loop = std::bind( &EventLoop::loop, subReactors_[i] );
        thpool_->add( sub_loop );
    }
}

Server::~Server()
{
    delete acceptor_;
    delete thpool_;
    for ( auto & it : subReactors_ ) {
        delete it;
    }
}

void Server::newConnection( Socket * s )
{
    if ( s->getFd() != -1 ) {
        int rand = s->getFd() % subReactors_.size();
        Connection * conn = new Connection( subReactors_[rand], s );
        DeleteConnectionCallback cb = std::bind( &Server::deleteConnection, this, std::placeholders::_1 );
        conn->setDeleteConnectionCallback( cb );
        connections_[s->getFd()] = conn;
    }
}

void Server::deleteConnection( Socket * s )
{
    auto it = connections_.find( s->getFd() );
    if ( it != connections_.end() ) {
        delete it->second;
        connections_.erase( it );
    }
}

