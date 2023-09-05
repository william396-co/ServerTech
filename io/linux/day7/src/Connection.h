#pragma once

#include <functional>
#include <memory>
#include "util.h"

class Socket;
class EventLoop;
class Channel;
class Connection
{
public:
    Connection( EventLoop * _loop, Socket * _s );
    ~Connection(){}

    void echo();
    void setDeleteConnectionCallback( ConnectionCallback cb )
    {
        deleteCallback = cb;
    }

private:
    EventLoop * loop;
    std::unique_ptr<Socket> s;
    std::unique_ptr<Channel> channel;
    ConnectionCallback deleteCallback;
};
