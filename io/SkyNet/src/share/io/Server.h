#pragma once
#include <unordered_map>
#include <vector>

class EventLoop;
class Acceptor;
class Socket;
class ThreadPool;
class Connection;
class Server
{
    using ConnectionMap = std::unordered_map<int, Connection *>;
    using ReactorList = std::vector<EventLoop *>;

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
    ConnectionMap connections_;
    ThreadPool * thpool_ {};
};
