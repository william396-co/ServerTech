#pragma once

#include "Macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#ifdef OS_MACOS
#include <sys/event.h>
#endif

#include <vector>

class Channel;
class Poller
{
public:
    Poller();
    ~Poller();

    DISALLOW_COPY_AND_MOVE( Poller );

    std::vector<Channel *> poll( int timeout = -1 );
    void updateChannel( Channel * ch );
    void deleteChannel( Channel * ch );

private:
    int fd_ { -1 };
#ifdef OS_LINUX
    struct epoll_event * events_ { nullptr };
#endif

#ifdef OS_MACOS
    struct kevent * events_ { nullptr };
#endif
};
