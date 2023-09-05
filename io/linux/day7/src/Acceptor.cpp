#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"
#include "EventLoop.h"

Acceptor::Acceptor( EventLoop * loop, const char * ip, uint16_t port )
{
    s = std::make_unique<Socket>();
    addr = std::make_unique<InetAddress>( ip, port );
    s->bind( *addr );
    s->listen();
    s->setnonblocking();

    acceptChannel = std::make_unique<Channel>( loop, s->getFd() );
    ChannelCallback cb = std::bind( &Acceptor::acceptConnection, this );
    acceptChannel->setCallback( cb );
    acceptChannel->enableReading();
}

Acceptor::~Acceptor()
{
}
