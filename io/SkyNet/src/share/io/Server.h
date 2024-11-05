#pragma once
#include <unordered_map>

class EventLoop;
class Acceptor;
class Socket;
class Connection;
class Server
{
    using ConnectionMap = std::unordered_map<int, Connection *>;

public:
    Server( EventLoop * loop, char * port );
    ~Server();

    void handleReadEvent( int fd );
    void newConnection( Socket * s );
    void deleteConnection( Socket * s );

private:
    void clear();

private:
    EventLoop * loop_ {};
    Acceptor * acceptor_ {};
    ConnectionMap connections_;
};
