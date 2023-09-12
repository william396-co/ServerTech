#pragma once

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif
#include <vector>
#include "Macros.h"

class Channel;
class Epoll
{
public:
    Epoll();
    ~Epoll();

    DISALLOW_COPY_AND_MOVE( Epoll );

    void addFd( int fd, uint32_t op );
    //    std::vector<epoll_event> poll( int timeout = -1 );
    std::vector<Channel *> poll( int timeout = -1 );
    void updateChannel( Channel * ch );
    void deleteChannel( Channel * ch );

private:
    int epfd;
    struct epoll_event * events;
};
