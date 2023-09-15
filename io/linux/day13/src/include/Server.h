#pragma once

#include <unordered_map>
#include <vector>
#include "Macros.h"

class EventLoop;
class Socket;
class Channel;
class Acceptor;
class Connection;
class ThreadPool;
using ConnectionList = std::unordered_map<int, Connection *>;

class Server
{
public:
    Server( EventLoop * loop, const char * ip, uint16_t port );
    ~Server();

    DISALLOW_COPY_AND_MOVE( Server );

    void handleReadEvent( int fd );
    void newConnection( Socket * _s );
    void deleteConnection( int fd );

private:
    EventLoop * mainReactor;
    Acceptor * acceptor;
    ConnectionList connections;
    std::vector<EventLoop *> subReactors;
    ThreadPool * thpool;
};
