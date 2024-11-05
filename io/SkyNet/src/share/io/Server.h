#pragma once
#include <unordered_map>

class EventLoop;
class Socket;
class Channel;
class Acceptor;
class Server
{
    using SocketChannelMap = std::unordered_map<int, std::pair<Socket *, Channel *>>;

public:
    Server( EventLoop * loop, char * port );
    ~Server();

    void handleReadEvent( int fd );
    void newConnection( Socket * listenSocket );

private:
    void addSocketChannel( int fd, Socket * s, Channel * ch );
    void delSocketChannel( int fd );

    void clear();

private:
    EventLoop * loop_ {};
    Acceptor * acceptor_ {};
    SocketChannelMap socketList_;
};
