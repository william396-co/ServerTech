#pragma once

#include <cstdint>
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
    void deleteConnection( Socket * _s );
    void onConnect( ConnectCallback && fn );
    void onMessage( MessageCallback && fn );
    void newConnect( NewConnectCallback && fn );

private:
    EventLoop * main_reactor_;
    Acceptor * acceptor_;
    ConnectionList connections_;
    std::vector<EventLoop *> sub_reactors_;
    ThreadPool * thread_pool_;
    ConnectCallback on_connect_callback_;
    MessageCallback on_message_callback_;
    NewConnectCallback new_connect_callback_;
};
