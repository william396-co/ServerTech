#pragma once

#include <unordered_map>
#include <vector>
#include "Macros.h"
#include "Types.h"

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

    void newConnection( Socket * _s );
    void deleteConnection( int fd );
    void onConnected( ConnectedCallback && fn );

private:
    EventLoop * mainReactor_;
    Acceptor * acceptor_;
    ConnectionList connections_;
    std::vector<EventLoop *> subReactors_;
    ThreadPool * thpool_;
    ConnectedCallback on_connected_callback_;
};
