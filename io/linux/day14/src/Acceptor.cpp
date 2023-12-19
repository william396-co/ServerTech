#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Server.h"
#include "Socket.h"

Acceptor::Acceptor( EventLoop * loop, const char * ip, uint16_t port )
{
    listenSock = new Socket();
    InetAddress addr( ip, port );
    listenSock->bind( addr );
    listenSock->listen();
    //  listenSock->setnonblocking(); //Acceptor use blocking IO is better

    acceptChannel = new Channel( loop, listenSock );
    ChannelReadCallback cb = std::bind( &Acceptor::acceptConnection, this );
    acceptChannel->setReadCallback( cb );
    acceptChannel->enableRead();
}

Acceptor::~Acceptor()
{
    delete listenSock;
    delete acceptChannel;
}

void Acceptor::acceptConnection()
{
    InetAddress cAddr {};
    Socket * client = new Socket( listenSock->accept( cAddr ) );
    printf( "new client fd %d! Ip:%s Port: %d\n", client->getFd(), cAddr.getIp(), cAddr.getPort() );
    client->setNonBlocking();
    if ( new_connection_callback_ ) {
        new_connection_callback_( client );
    }
}
