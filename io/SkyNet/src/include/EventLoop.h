#pragma once

#include <functional>

class Epoll;
class Channel;
class ThreadPool;
class EventLoop {
 public:
  EventLoop();
  ~EventLoop();

  void loop();
  void updateChannel(Channel* ch);

  void addThread(std::function<void()> task);

 private:
  Epoll* ep_{};
  ThreadPool* pool_{};
  bool quit{};
};
