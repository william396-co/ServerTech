#pragma once

#include <functional>
#include <memory>
#include "util.h"

class Socket;
class EventLoop;
class Channel;
class Buffer;
class Connection
{
public:
    Connection( EventLoop * _loop, Socket * _s );
    ~Connection();
    void echo( int fd );
    void setDeleteConnectionCallback( ConnectionCallback cb )
    {
        deleteCallback = cb;
    }

private:
    EventLoop * loop;
    Socket * s;
    Channel * channel;
    Buffer * readBuffer;
    Buffer * writeBuffer;
    ConnectionCallback deleteCallback;
};
