#include "Acceptor.h"

#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"

Acceptor::Acceptor( EventLoop * loop, char * port )
    : loop_ { loop }
{

    listenSocket_ = new Socket();
    listenSocket_->listen( port );
    listenSocket_->setnonblocking();

    acceptChannel_ = new Channel( loop, listenSocket_->getFd() );
    std::function<void()> cb = std::bind( &Acceptor::acceptConnection, this );
    acceptChannel_->setCallback( cb );
    acceptChannel_->enableReading();
}

Acceptor::~Acceptor()
{
    delete listenSocket_;
    delete acceptChannel_;
}

void Acceptor::acceptConnection()
{
    if ( newConnCallback_ ) {
        newConnCallback_( listenSocket_ );
    }
}
