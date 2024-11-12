#pragma once
#include <functional>

#include "Macros.h"
#include "Types.h"

class EventLoop;
class Channel;
class Socket;

class Acceptor {
   public:
    Acceptor(EventLoop* loop_, char* port);
    ~Acceptor();

    DISALLOW_COPY_AND_MOVE(Acceptor);

    void AcceptConnection();
    void SetNewConnectionCallback(ConnectionCallback const& cb) { new_connection_callback_ = cb; }

   private:
    EventLoop* loop_{};
    Socket* listenSocket_{};
    Channel* acceptChannel_{};
    ConnectionCallback new_connection_callback_{};
};
