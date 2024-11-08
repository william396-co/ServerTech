#pragma once

#include <functional>
#include <string>

#include "Macros.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;

using DeleteConnectionCallback = std::function<void(Socket*)>;

class Connection {
   public:
    Connection(EventLoop* loop, Socket* s);
    ~Connection();

    DISALLOW_COPY_AND_MOVE(Connection);

    void Echo(int fd);
    void SetDeleteConnectionCallback(DeleteConnectionCallback const& cb) { deleteConnectionCallback_ = cb; }
    void Send(int fd);

   private:
    EventLoop* loop_{};
    Socket* s_;
    Channel* ch_;
    DeleteConnectionCallback deleteConnectionCallback_{};
    Buffer* writeBuffer_{};
    Buffer* readBuffer_{};
};
