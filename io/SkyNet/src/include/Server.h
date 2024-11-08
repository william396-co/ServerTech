#pragma once

#include <functional>
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
    using OnConnectCallback = std::function<void(Connection*)>;

   public:
    Server(EventLoop* loop, char* port);
    ~Server();

    DISALLOW_COPY_AND_MOVE(Server);

    void NewConnection(Socket* s);
    void DeleteConnection(Socket* s);
    void OnConnect(OnConnectCallback fn) { on_connect_callback_ = std::move(fn); }

   private:
    EventLoop* mainReactor_{};
    Acceptor* acceptor_{};
    ReactorList subReactors_{};
    ThreadList thpool_;
    ConnectionMap connections_;
    OnConnectCallback on_connect_callback_{};
};
