#include "Server.h"

#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

#include <unistd.h>
#include <cassert>
#include <cstring>
#include <functional>
#include <thread>

constexpr auto READ_BUFFER = 1024;

Server::Server( EventLoop * _loop, const char * ip, uint16_t port )
    : mainReactor_ { _loop }, acceptor_ { nullptr }
{
    acceptor_ = new Acceptor { mainReactor_, ip, port };
    NewConnectionCallback cb = std::bind( &Server::newConnection, this, std::placeholders::_1 );
    acceptor_->setNewConnectionCallback( cb );

    int size = std::thread::hardware_concurrency();
    thpool_ = new ThreadPool( size );
    for ( int i = 0; i < size; ++i ) {
        subReactors_.emplace_back( new EventLoop() );
    }

    for ( int i = 0; i < size; ++i ) {
        std::function<void()> sub_loop = std::bind( &EventLoop::loop, subReactors_[i] );
        thpool_->add( sub_loop );
    }
}

Server::~Server()
{
    for ( auto & it : connections_ ) {
        delete it.second;
    }

    delete acceptor_;
    delete thpool_;

    for ( auto & it : subReactors_ ) {
        delete it;
    }
}

void Server::deleteConnection( int fd )
{
    auto it = connections_.find( fd );
    if ( it != connections_.end() ) {
        delete it->second;
        connections_.erase( it );
    }
}

void Server::newConnection( Socket * _s )
{
    if ( _s->getFd() != -1 ) {
        int random = _s->getFd() % subReactors_.size();
        Connection * conn = new Connection( subReactors_[random], _s );
        DeleteConnectionCallback cb = std::bind( &Server::deleteConnection, this, std::placeholders::_1 );
        conn->setDeleteConnectionCallback( cb );
        // conn->setOnConnectedCallback( on_connected_callback_ );
        conn->setOnMessageCallback( on_message_callback_ );
        connections_.emplace( _s->getFd(), conn );
        if ( new_connect_callback_ ) {
            new_connect_callback_( conn );
        }
    }
}

void Server::onConnect( ConnectedCallback && fn )
{
    on_connected_callback_ = std::move( fn );
}

void Server::onMessage( MessageCallback && fn )
{
    on_message_callback_ = std::move( fn );
}

void Server::onNewConnect( NewConnectCallback && fn )
{
    new_connect_callback_ = std::move( fn );
}
