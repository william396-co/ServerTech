#pragma once

#include <functional>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Macros.h"
#include "Types.h"

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

    void OnConnect(ConnectionMessageCallback fn) { on_connect_callback_ = std::move(fn); }
    void OnMessage(ConnectionMessageCallback fn) { on_message_callback_ = std::move(fn); }
    void NewConnect(ConnectionMessageCallback fn) { new_connect_callback_ = std::move(fn); }

   private:
    EventLoop* mainReactor_{};
    Acceptor* acceptor_{};
    ReactorList subReactors_{};
    ThreadList thpool_;

    ConnectionMap connections_;

    ConnectionMessageCallback on_connect_callback_{};
    ConnectionMessageCallback on_message_callback_{};
    ConnectionMessageCallback new_connect_callback_{};
};
