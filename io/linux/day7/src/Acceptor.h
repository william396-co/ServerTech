#pragma once

#include <functional>
#include <memory>
#include "util.h"

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
public:
    Acceptor( EventLoop * _loop, const char * ip, uint16_t port );
    ~Acceptor();
    void acceptConnection()
    {
        if ( newConCallback ) newConCallback( s.get() );
    }
    void setNewConnectionCallback( ConnectionCallback cb )
    {
        newConCallback = cb;
    }

private:
    std::unique_ptr<EventLoop> loop;
    std::unique_ptr<Socket> s;
    std::unique_ptr<InetAddress> addr;
    std::unique_ptr<Channel> acceptChannel;
    ConnectionCallback newConCallback = nullptr;
};
