#pragma once

#include <functional>

class EventLoop;
class Channel;
class Socket;

using NewConnCallback = std::function<void(Socket*)>;
class Acceptor {
 public:
  Acceptor(EventLoop* loop_, char* port);
  ~Acceptor();

  void acceptConnection();
  void setNewConnectionCallback(NewConnCallback cb) { newConnCallback_ = cb; }

 private:
  EventLoop* loop_{};
  Socket* listenSocket_{};
  Channel* acceptChannel_{};
  NewConnCallback newConnCallback_{};
};
