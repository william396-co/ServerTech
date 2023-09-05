#pragma once

#include <unordered_map>
#include <memory>

class EventLoop;
class Socket;
class Channel;
class Acceptor;
class Connection;
using ConnectionList = std::unordered_map<int, Connection *>;

class Server
{
public:
    Server( EventLoop * loop, const char * ip, uint16_t port );
    ~Server();

    void handleReadEvent( int fd );
    void newConnection( Socket * listenfd );

private:
    void addSocketChannel( int fd, Socket * s, Channel * ch );
    void delSocketChannel( int fd );
    void clear();

private:
    EventLoop * loop;
    Acceptor * acceptor;
    ConnectionList connections;
};
