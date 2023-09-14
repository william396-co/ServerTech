#pragma once

#include <functional>
#include <memory>
#include "util.h"
#include "Macros.h"

class Socket;
class EventLoop;
class Channel;
class Buffer;
class Connection
{
public:
    Connection( EventLoop * _loop, Socket * _s );
    ~Connection();

    DISALLOW_COPY_AND_MOVE( Connection );

    void echo( int fd );
    inline void setDeleteConnectionCallback( DeleteConnectionCallback cb )
    {
        deleteCallback = cb;
    }

    void send( int fd );

private:
    EventLoop * loop;
    Socket * s;
    Channel * channel;
    Buffer * readBuffer;
    Buffer * writeBuffer;
    DeleteConnectionCallback deleteCallback;
};
