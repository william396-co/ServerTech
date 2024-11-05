#pragma once

#include <functional>

class EventLoop;
class Socket;
class Channel;

using DeleteConnectionCallback = std::function<void( Socket * )>;
class Connection
{
public:
    Connection( EventLoop * loop, Socket * s );
    ~Connection();

    void echo( int fd );
    void setDeleteConnectionCallback( DeleteConnectionCallback const & cb ) { deleteConnectionCallback_ = cb; }

private:
    EventLoop * loop_ {};
    Socket * s_;
    Channel * ch_;
    DeleteConnectionCallback deleteConnectionCallback_ {};
};
