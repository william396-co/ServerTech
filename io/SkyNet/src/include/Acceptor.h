#pragma once
#include <functional>

#include "Macros.h"

class EventLoop;
class Channel;
class Socket;

using NewConnCallback = std::function<void(Socket*)>;
class Acceptor {
   public:
    Acceptor(EventLoop* loop_, char* port);
    ~Acceptor();

    DISALLOW_COPY_AND_MOVE(Acceptor);

    void AcceptConnection();
    void SetNewConnectionCallback(NewConnCallback cb) { newConnCallback_ = cb; }

   private:
    EventLoop* loop_{};
    Socket* listenSocket_{};
    Channel* acceptChannel_{};
    NewConnCallback newConnCallback_{};
};
