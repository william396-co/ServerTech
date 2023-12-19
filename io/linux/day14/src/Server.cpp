#include "Server.h"

#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"
#include "Exception.h"

#include <unistd.h>
#include <cassert>
#include <cstring>
#include <functional>
#include <thread>

constexpr auto READ_BUFFER = 1024;

Server::Server( EventLoop * _loop, const char * ip, uint16_t port )
    : main_reactor_ { _loop }, acceptor_ { nullptr }
{
    if ( !main_reactor_ ) {
        throw Exception( ExceptionType::INVALID, "main reactor can't be nullptr!" );
    }

    acceptor_ = new Acceptor { main_reactor_, ip, port };
    NewConnectionCallback cb = std::bind( &Server::newConnection, this, std::placeholders::_1 );
    acceptor_->setNewConnectionCallback( cb );

    int size = std::thread::hardware_concurrency();
    thread_pool_ = new ThreadPool( size );
    for ( int i = 0; i < size; ++i ) {
        sub_reactors_.emplace_back( new EventLoop() );
    }

    for ( int i = 0; i < size; ++i ) {
        std::function<void()> sub_loop = std::bind( &EventLoop::loop, sub_reactors_[i] );
        thread_pool_->add( std::move( sub_loop ) );
    }
}

Server::~Server()
{
    for ( auto & it : connections_ ) {
        delete it.second;
    }

    delete acceptor_;
    delete thread_pool_;

    for ( auto & it : sub_reactors_ ) {
        delete it;
    }
}

void Server::deleteConnection( Socket * _s )
{
    auto fd = _s->getFd();
    auto it = connections_.find( fd );
    if ( it != connections_.end() ) {
        delete it->second;
        connections_.erase( it );
    }
}

void Server::newConnection( Socket * _s )
{
    if ( _s->getFd() == -1 ) {
        throw Exception( ExceptionType::INVALID_SOCKET, "New Connection error, invalid client socket!" );
    }
    int random = _s->getFd() % sub_reactors_.size();
    Connection * conn = new Connection( sub_reactors_[random], _s );
    DeleteConnectionCallback cb = std::bind( &Server::deleteConnection, this, std::placeholders::_1 );
    conn->setDeleteConnectionCallback( cb );
    // conn->setOnConnectedCallback( on_connected_callback_ );
    conn->setOnMessageCallback( on_message_callback_ );
    connections_.emplace( _s->getFd(), conn );
    if ( new_connect_callback_ ) {
        new_connect_callback_( conn );
    }
}

void Server::onConnect( ConnectCallback && fn )
{
    on_connect_callback_ = std::move( fn );
}

void Server::onMessage( MessageCallback && fn )
{
    on_message_callback_ = std::move( fn );
}

void Server::newConnect( NewConnectCallback && fn )
{
    new_connect_callback_ = std::move( fn );
}
