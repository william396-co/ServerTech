#pragma once

#include <functional>
#include "Macros.h"
#include "Types.h"

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
public:
    Acceptor( EventLoop * _loop, const char * ip, uint16_t port );
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback( NewConnectionCallback const & cb ) { newConnectionCallback = cb; }
    DISALLOW_COPY_AND_MOVE( Acceptor );

private:
    EventLoop * loop;
    Socket * listenSock;
    Channel * acceptChannel;
    NewConnectionCallback newConnectionCallback;
};
