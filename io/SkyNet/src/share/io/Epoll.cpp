#include "Epoll.h"

#include "utils/util.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

constexpr auto MAX_EVENTS = 1000;

Epoll::Epoll()
    : epfd { -1 }, events {}
{
    epfd = epoll_create1( 0 );
    errif( epfd == -1, "epoll create error" );
    events = new epoll_event[MAX_EVENTS];
    bzero( events, sizeof( *events ) * MAX_EVENTS );
}

Epoll::~Epoll()
{
    if ( epfd != -1 ) {
        std::cout << __FUNCTION__ << " epfd:" << epfd << "\n";
        close( epfd );
    }
    delete[] events;
}

void Epoll::addFd( int fd, uint32_t op )
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = op;
    errif( epoll_ctl( epfd, EPOLL_CTL_ADD, fd, &ev ), "epoll add event error" );
}

std::vector<epoll_event> Epoll::poll( int timeout )
{
    int nfds = epoll_wait( epfd, events, MAX_EVENTS, timeout );

    errif( nfds == -1, "epoll wait error" );

    std::vector<epoll_event> res( nfds );
    for ( int i = 0; i != nfds; ++i ) {
        res[i] = events[i];
    }
    return res;
}
