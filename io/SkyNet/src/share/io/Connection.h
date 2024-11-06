#pragma once

#include <functional>
#include <string>

class EventLoop;
class Socket;
class Channel;
class Buffer;

using DeleteConnectionCallback = std::function<void( Socket * )>;
class Connection
{
public:
    Connection( EventLoop * loop, Socket * s );
    ~Connection();

    void echo( int fd );
    void setDeleteConnectionCallback( DeleteConnectionCallback const & cb ) { deleteConnectionCallback_ = cb; }
    void send( int fd );

private:
    EventLoop * loop_ {};
    Socket * s_;
    Channel * ch_;
    DeleteConnectionCallback deleteConnectionCallback_ {};
    Buffer * writeBuffer_ {};
    Buffer * readBuffer_ {};
};
