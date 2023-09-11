#pragma once

#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
public:
    Epoll();
    ~Epoll();

    void addFd( int fd, uint32_t op );
    //    std::vector<epoll_event> poll( int timeout = -1 );
    std::vector<Channel *> poll( int timeout = -1 );
    void updateChannel( Channel * ch );
    void deleteChannel( Channel * ch );

private:
    int epfd;
    struct epoll_event * events;
};
