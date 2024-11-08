#pragma once

#include <thread>
#include <unordered_map>
#include <vector>

#include "Macros.h"

class EventLoop;
class Acceptor;
class Socket;
class Connection;
class Server {
    using ConnectionMap = std::unordered_map<int, Connection*>;
    using ReactorList = std::vector<EventLoop*>;
    using ThreadList = std::vector<std::thread>;

   public:
    Server(EventLoop* loop, char* port);
    ~Server();

    DISALLOW_COPY_AND_MOVE(Server);

    void NewConnection(Socket* s);
    void DeleteConnection(Socket* s);

   private:
    EventLoop* mainReactor_{};
    Acceptor* acceptor_{};
    ReactorList subReactors_{};
    ThreadList thpool_;
    ConnectionMap connections_;
};
