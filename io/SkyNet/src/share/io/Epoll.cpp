#include "Epoll.h"

#include "Channel.h"
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

void Epoll::updateChannel( Channel * ch )
{
    int fd = ch->getFd();
    struct epoll_event ev;
    ev.data.ptr = ch;
    ev.events = ch->getEvents();
    if ( !ch->isInEpoll() ) {
        errif( epoll_ctl( epfd, EPOLL_CTL_ADD, fd, &ev ) == -1, "epoll add error" );
        ch->setInEpoll();
    } else {
        errif( epoll_ctl( epfd, EPOLL_CTL_MOD, fd, &ev ) == -1, "epoll modify error" );
    }
}

std::vector<Channel *> Epoll::poll( int timeout )
{
    int nfds = epoll_wait( epfd, events, MAX_EVENTS, timeout );
    errif( nfds == -1, "epoll wait error" );
    std::vector<Channel *> res( nfds );
    for ( int i = 0; i != nfds; ++i ) {
        Channel * ch = static_cast<Channel *>( events[i].data.ptr );
        ch->setRevents( events[i].events );
        res[i] = ch;
    }
    return res;
}
