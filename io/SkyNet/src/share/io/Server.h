#pragma once
#include <unordered_map>
#include <vector>
#include <thread>

class EventLoop;
class Acceptor;
class Socket;
class Connection;
class Server
{
    using ConnectionMap = std::unordered_map<int, Connection *>;
    using ReactorList = std::vector<EventLoop *>;
    using ThreadList = std::vector<std::thread>;

public:
    Server( EventLoop * loop, char * port );
    ~Server();

    void handleReadEvent( int fd );
    void newConnection( Socket * s );
    void deleteConnection( Socket * s );

private:
    void clear();

private:
    EventLoop * mainReactor_ {};
    Acceptor * acceptor_ {};
    ReactorList subReactors_ {};
    ThreadList thpool_;
    ConnectionMap connections_;
};
