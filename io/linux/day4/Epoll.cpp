#include "Epoll.h"

#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>

constexpr auto MAX_EVENTS = 1000;

Epoll::Epoll()
    : epfd { -1 }, events { nullptr }
{
    epfd = epoll_create1( 0 );
    errif( epfd == -1, "epoll create error" );
    events = new epoll_event[MAX_EVENTS];
    bzero( events, sizeof( *events ) * MAX_EVENTS );
}

Epoll::~Epoll()
{
    if ( epfd != -1 ) {
        close( epfd );
    }
    delete[] events;
}

void Epoll::addFd( int fd, uint32_t op )
{
    struct epoll_event ev;
    bzero( &ev, sizeof( ev ) );
    ev.data.fd = fd;
    ev.events = op;
    errif( epoll_ctl( epfd, EPOLL_CTL_ADD, fd, &ev ) == -1, "epoll add event error" );
}

/*
std::vector<epoll_event> Epoll::poll( int timeout )
{
    int nfds = epoll_wait( epfd, events, MAX_EVENTS, timeout );
    errif( nfds == -1, "epoll wait error" );
    std::vector<epoll_event> activeEvents( nfds );
    for ( int i = 0; i != nfds; ++i ) {
        activeEvents[i] = events[i];
    }
    return activeEvents;
}*/

std::vector<Channel *> Epoll::poll( int timeout )
{
    int nfds = epoll_wait( epfd, events, MAX_EVENTS, timeout );
    errif( nfds == -1, "epoll wait error" );
    std::vector<Channel *> activeChannels;
    for ( int i = 0; i != nfds; ++i ) {
        Channel * ch = (Channel *)events[i].data.ptr;
        ch->setRevents( events[i].events );
        activeChannels.emplace_back( ch );
    }
    return activeChannels;
}

void Epoll::updateChannel( Channel * ch )
{
    int fd = ch->getFd();
    struct epoll_event ev;
    bzero( &ev, sizeof( ev ) );
    ev.data.ptr = ch;
    ev.events = ch->getEvents();
    if ( !ch->getInEpoll() ) {
        errif( epoll_ctl( epfd, EPOLL_CTL_ADD, fd, &ev ) == -1, "epoll add error" );
        ch->setInEpoll();
    } else {
        errif( epoll_ctl( epfd, EPOLL_CTL_MOD, fd, &ev ) == -1, "epoll modify error" );
    }
}
