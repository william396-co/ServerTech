#include "Poller.h"

#include <unistd.h>
#include <cstring>
#include "Channel.h"
#include "util.h"
#include "Socket.h"

constexpr auto MAX_EVENTS = 1000;

#ifdef OS_LINUX

Poller::Poller()
    : fd_ { -1 }, events_ { nullptr }
{
    fd_ = epoll_create1( 0 );
    ErrIf( fd_ == -1, "epoll create error" );
    events_ = new epoll_event[MAX_EVENTS];
    memset( events_, 0, sizeof( *events_ ) * MAX_EVENTS );
}

Poller::~Poller()
{
    if ( fd_ != -1 ) {
        close( fd_ );
    }
    delete[] events_;
}

std::vector<Channel *> Poller::poll( int timeout )
{
    int nfds = epoll_wait( fd_, events_, MAX_EVENTS, timeout );
    ErrIf( nfds == -1, "epoll wait error" );
    std::vector<Channel *> activeChannels;
    for ( int i = 0; i != nfds; ++i ) {
        Channel * ch = (Channel *)events_[i].data.ptr;
        int events = events_[i].events;
        if ( events & EPOLLIN ) {
            ch->setReadyEvents( Channel::READ_EVENT );
        }
        if ( events & EPOLLOUT ) {
            ch->setReadyEvents( Channel::WRITE_EVENT );
        }
        if ( events & EPOLLET ) {
            ch->setReadyEvents( Channel::ET );
        }
        activeChannels.emplace_back( ch );
    }
    return activeChannels;
}

void Poller::updateChannel( Channel * ch )
{
    int fd = ch->getSocket()->getFd();
    struct epoll_event ev
    {};
    ev.data.ptr = ch;
    if ( ch->getListenEvents() & Channel::READ_EVENT ) {
        ev.events |= EPOLLIN | EPOLLPRI;
    }
    if ( ch->getListenEvents() & Channel::WRITE_EVENT ) {
        ev.events |= EPOLLOUT;
    }
    if ( ch->getListenEvents() & Channel::ET ) {
        ev.events |= EPOLLET;
    }
    if ( !ch->getExist() ) {
        ErrIf( epoll_ctl( fd_, EPOLL_CTL_ADD, fd, &ev ) == -1, "epoll add error" );
        ch->setExist();
    } else {
        ErrIf( epoll_ctl( fd_, EPOLL_CTL_MOD, fd, &ev ) == -1, "epoll modify error" );
    }
}

void Poller::deleteChannel( Channel * ch )
{
    int fd = ch->getSocket()->getFd();
    ErrIf( epoll_ctl( fd_, EPOLL_CTL_DEL, fd, NULL ) == -1, "epoll delete error" );
    ch->setExist( false );
}
#endif

#ifdef MAC_OS

Poller::Poller()
    : fd_ { -1 }, events_ { nullptr }
{
    fd_ = kqueue( 0 );
    ErrIf( fd_ == -1, "kqueue create error" );
    events_ = new struct kevent[MAX_EVENTS];
    memset( events_, 0, sizeof( *events_ ) * MAX_EVENTS );
}

Poller::~Poller()
{
    if ( fd_ != -1 ) {
        close( fd_ );
    }
    delete[] events_;
}

std::vector<Channel *> Poller::poll( int timeout )
{
    struct timespec ts;
    memset( &ts, 0, sizeof( ts ) );
    if ( timeout != -1 ) {
        ts.tv_sec = timeout / 1000;
        ts.tv_nsec = ( timeout % 1000 ) * 1000 * 1000;
    }
    int nfds = 0;
    if ( timeout == -1 ) {
        nfds = kevent( fd_, NULL, 0, events_, MAX_EVENTS, NULL );
    } else {
        nfds = kevent( fd_, NULL, 0, events_, MAX_EVENTS, &ts );
    }
    ErrIf( nfds == -1, "epoll wait error" );
    std::vector<Channel *> activeChannels;
    for ( int i = 0; i != nfds; ++i ) {
        Channel * ch = (Channel *)events_[i].data.ptr;
        int events = events_[i].filter;
        if ( events & EVFILT_READ ) {
            ch->setReadyEvents( Channel::READ_EVENT | Channel::ET );
        }
        if ( events & ) {
            ch->setReadyEvents( Channel::WRITE_EVENT | Channel::ET );
        }
        activeChannels.emplace_back( ch );
    }
    return activeChannels;
}

void Poller::updateChannel( Channel * ch )
{
    struct kevent ev[2];
    memset( ev, 0, sizeof( *ev ) * 2 );
    int n = 0;
    int fd = ch->getSocket()->getFd();
    int op = EV_ADD;
    if ( ch->getListenEvents() & Channel::ET ) {
        op |= EV_ADD;
    }
    if ( ch->getListenEvents() & Channel::READ_EVENT ) {
        EV_SET( &ev[n++], fd, EVFILT_READ, op, 0, 0, ch );
    }
    if ( ch->getListenEvents() & Channel::WRITE_EVENT ) {
        EV_SET( &ev[n++], fd, EVFILT_WRITE, op, 0, 0, ch );
    }
    int r = kevent( fd_, ev, n, NULL, 0, NULL );
    ErrIf( r == -1, "Kqueue add event error" );
}

void Poller::deleteChannel( Channel * ch )
{
    struct kevent ev[2];
    int n = 0;
    int fd = ch->getSocket()->getFd();
    if ( ch->getListenEvents() & Channel::READ_EVENT ) {
        EV_SET( &ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, ch );
    }
    if ( ch->getListenEvents() & Channel::WRITE_EVENT ) {
        EV_SET( &ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, ch );
    }
    int r = kevent( fd_, ev, n, NULL, 0, NULL );
    ErrIf( r == -1, "Kqueue delete event error" );
}

#endif
