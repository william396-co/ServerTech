#include "Acceptor.h"

#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"

#include <iostream>

Acceptor::Acceptor( EventLoop * loop, char * port )
    : loop_ { loop }
{

    listenSocket_ = new Socket();
    listenSocket_->listen( port );
    listenSocket_->setnonblocking();

    acceptChannel_ = new Channel( loop, listenSocket_->getFd() );
    std::function<void()> cb = std::bind( &Acceptor::acceptConnection, this );
    acceptChannel_->setReadCallback( cb );
    acceptChannel_->enableReading();
    acceptChannel_->setUseThreadPool( false );
}

Acceptor::~Acceptor()
{
    delete listenSocket_;
    delete acceptChannel_;
}

void Acceptor::acceptConnection()
{
    // Accept new Socket
    sockaddr_in addr {};
    Socket * clientSocket = new Socket( listenSocket_->accept( addr ) );
    clientSocket->setRemote( addr );
    std::cout << "new client fd:" << clientSocket->getFd() << " Ip:" << clientSocket->remoteIp() << " Port:" << clientSocket->remotePort() << "\n";
    clientSocket->setnonblocking();

    if ( newConnCallback_ ) {
        newConnCallback_( clientSocket );
    }
}
