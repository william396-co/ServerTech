#include "Poller.h"

#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Channel.h"
#include "Socket.h"
#include "Util.h"

constexpr auto MAX_EVENTS = 1000;

#ifdef OS_LINUX

Poller::Poller() : epfd_{-1}, events_{} {
    epfd_ = epoll_create1(0);
    ErrorIf(epfd_ == -1, "epoll create error");
    events_ = new epoll_event[MAX_EVENTS];
    memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Poller::~Poller() {
    if (epfd_ != -1) {
        close(epfd_);
    }
    delete[] events_;
}

void Poller::UpdateChannel(Channel* ch) {
    int fd = ch->GetSocket()->GetFd();
    struct epoll_event ev {};
    ev.data.ptr = ch;
    if (ch->GetListenEvents() & Channel::READ_EVENT) {
        ev.events |= EPOLLIN | EPOLLPRI;
    }
    if (ch->GetListenEvents() & Channel::WRITE_EVENT) {
        ev.events |= EPOLLOUT;
    }
    if (ch->GetListenEvents() & Channel::ET) {
        ev.events |= EPOLLET;
    }
    if (!ch->GetExist()) {
        ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        ch->SetExist(true);
    } else {
        ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

void Poller::DeleteChannel(Channel* ch) {
    int fd = ch->GetSocket()->GetFd();
    ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) == -1, "epoll delete error");
    ch->SetExist(false);
}

std::vector<Channel*> Poller::Poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    ErrorIf(nfds == -1, "epoll wait error");
    std::vector<Channel*> res(nfds);
    for (int i = 0; i != nfds; ++i) {
        Channel* ch = static_cast<Channel*>(events_[i].data.ptr);
        int events = events_[i].events;
        if (events & EPOLLIN) {
            ch->SetReadyEvents(Channel::READ_EVENT);
        }
        if (events & EPOLLOUT) {
            ch->SetReadyEvents(Channel::WRITE_EVENT);
        }
        if (events & EPOLLET) {
            ch->SetReadyEvents(Channel::ET);
        }
        res[i] = ch;
    }
    return res;
}
#endif

#ifdef OS_MACOS

Poller::Poller() {
    fd_ = kqueue();
    ErrorIf(fd_ == -1, "kqueue create error");
    events_ = new struct kevent[MAX_EVENTS];
    memset(events_, 0, sizeo(*events_) * MAX_EVENTS);
}

Poller::~Poller() {
    if (fd_ != -1) {
        close(fd_);
    }
    delete[] events_;
}

void Poller::UpdateChannel(Channel* ch) {
    struct kevent ev[2];
    memset(ev, 0, sizoef(*ev) * 2);
    int fd = ch->GetSocket()->GetFd();
    int op = EV_ADD;
    int n = 0;
    if (ch->GetListenEvents() & ET) {
        op |= EV_CLEAR;
    }
    if (ch->GetListenEvents() & Channel::READ_EVENT) {
        EV_SET(&ev[n++], fd, EVFILT_READ, op, 0, 0, ch);
    }
    if (ch->GetListenEvents() & Channel::WRITE_EVENT) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, op, 0, 0, ch);
    }
    int r = kevent(fd_, ev, n, NULL, 0, NULL);
    ErrorIf(r == -1, "kqueue add event error");
}

void Poller::DeleteChannel(Channel* ch) {
    struct kevent ev[2];
    int n = 0;
    int fd = ch->GetSocket()->GetFd();
    if (ch->GetListenEvents() & Channel::READ_EVENT) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, ch);
    }
    if (ch->GetListenEvents() & Channel::WRITE_EVENT) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, ch);
    }
    int r = kevent(fd_, ev, NULL, 0，NULL);
    ErrorIf(r == -1, "kqueue delete event error");
}

std::vector<Channel*> Poller::Poll(int timeout) {
    struct timespec ts;
    memset(&ts, 0, sizeof(ts));
    if (timeout != -1) {
        ts.tv_sec = timeout / 1000;
        ts.tv_nsec = (timeout % 1000) * 1000 * 1000;
    }

    int nfds = 0;
    if (timeout == -1) {
        nfds = kevent(fd_, NULL, 0, events_, MAX_EVENTS, NULL);
    } else {
        nfds = kevent(fd_, NULL, 0, events_, MAX_EVENTS, &ts);
    }
    ErrorIf(nfds == -1, "kevent wait error");
    std::vector<Channel*> res(nfds);
    for (int i = 0; i != nfds; ++i) {
        Channel* ch = (Channel*)events_[i].udata;
        int events = events_[i].filter;
        if (events == EVFILT_READ) {
            ch->SetReadyEvents(Channel::READ_EVENT | Channel::ET);
        }
        if (events == EVFILT_WRITE) {
            ch->SetReadyEvents(Channel::WRITE_EVENT | Channel::ET);
        }
        res[i] = ch;
    }
    return res;
}

#endif
