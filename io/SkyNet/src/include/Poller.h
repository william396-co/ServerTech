#pragma once

#include <vector>

#include "Macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#ifdef OS_MACOS
#include <sys/event.h>
#endif

class Channel;
class Poller {
   public:
    Poller();
    ~Poller();

    DISALLOW_COPY_AND_MOVE(Poller);

    int GetFd() const { return epfd_; }

    void UpdateChannel(Channel* ch);
    void DeleteChannel(Channel* ch);

    std::vector<Channel*> Poll(int timeout = -1);

   private:
    int epfd_{};
#ifdef OS_LINUX
    struct epoll_event* events_{};
#endif
#ifdef OS_MACOS
    struct kevent* events_{};
#endif
};
