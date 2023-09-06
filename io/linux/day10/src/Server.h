#pragma once

#include <unordered_map>

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
    void newConnection( Socket * _s );
    void deleteConnection( int fd );

private:
    EventLoop * loop;
    Acceptor * acceptor;
    ConnectionList connections;
};
