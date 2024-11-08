#pragma once

#include "Macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#include <vector>

class Channel;
class Epoll {
   public:
    Epoll();
    ~Epoll();

    DISALLOW_COPY_AND_MOVE(Epoll);

    int GetFd() const { return epfd_; }

    void UpdateChannel(Channel* ch);
    void DeleteChannel(Channel* ch);

    std::vector<Channel*> Poll(int timeout = -1);

   private:
    int epfd_{};
    struct epoll_event* events_{};
};
