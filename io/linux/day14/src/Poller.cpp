#include "Poller.h"

#include <unistd.h>
#include <cstring>
#include "Channel.h"
#include "util.h"

constexpr auto MAX_EVENTS = 1000;

#ifdef OS_LINUX

Poller::Poller()
    : fd_ { -1 }, events { nullptr }
{
    fd_ = epoll_create1( 0 );
    ErrIf( fd_ == -1, "epoll create error" );
    events = new epoll_event[MAX_EVENTS];
    bzero( events, sizeof( *events ) * MAX_EVENTS );
}

Poller::~Poller()
{
    if ( fd_ != -1 ) {
        close( fd_ );
    }
    delete[] events;
}

std::vector<Channel *> Poller::poll( int timeout )
{
    int nfds = epoll_wait( fd_, events, MAX_EVENTS, timeout );
    ErrIf( nfds == -1, "epoll wait error" );
    std::vector<Channel *> activeChannels;
    for ( int i = 0; i != nfds; ++i ) {
        Channel * ch = (Channel *)events[i].data.ptr;
        ch->setReady( events[i].events );
        activeChannels.emplace_back( ch );
    }
    return activeChannels;
}

void Poller::updateChannel( Channel * ch )
{
    int fd = ch->getFd();
    struct epoll_event ev;
    bzero( &ev, sizeof( ev ) );
    ev.data.ptr = ch;
    ev.events = ch->getEvents();
    if ( !ch->getInPoller() ) {
        ErrIf( epoll_ctl( fd_, EPOLL_CTL_ADD, fd, &ev ) == -1, "epoll add error" );
        ch->setInPoller();
    } else {
        ErrIf( epoll_ctl( fd_, EPOLL_CTL_MOD, fd, &ev ) == -1, "epoll modify error" );
    }
}

void Poller::deleteChannel( Channel * ch )
{
    int fd = ch->getFd();
    ErrIf( epoll_ctl( fd_, EPOLL_CTL_DEL, fd, NULL ) == -1, "epoll delete error" );
    ch->setInPoller( false );
}
#endif

#ifdef OS_MACOS

#endif
