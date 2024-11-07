#pragma once

#include <sys/epoll.h>

#include <vector>

class Channel;
class Epoll {
 public:
  Epoll();
  ~Epoll();

  int getFd() const { return epfd; }

  void updateChannel(Channel* ch);
  void deleteChannel(Channel* ch);

  std::vector<Channel*> poll(int timeout = -1);

 private:
  int epfd;
  struct epoll_event* events;
};
