#include "Epoll.h"

#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Channel.h"
#include "Util.h"

constexpr auto MAX_EVENTS = 1000;

Epoll::Epoll() : epfd_{-1}, events_{} {
    epfd_ = epoll_create1(0);
    ErrorIf(epfd_ == -1, "epoll create error");
    events_ = new epoll_event[MAX_EVENTS];
    memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd_ != -1) {
        close(epfd_);
    }
    delete[] events_;
}

void Epoll::UpdateChannel(Channel* ch) {
    int fd = ch->GetFd();
    struct epoll_event ev {};
    ev.data.ptr = ch;
    ev.events = ch->GetListenEvents();
    if (!ch->IsInEpoll()) {
        ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        ch->SetInEpoll(true);
    } else {
        ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

void Epoll::DeleteChannel(Channel* ch) {
    int fd = ch->GetFd();
    ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) == -1, "epoll delete error");
    ch->SetInEpoll(false);
}

std::vector<Channel*> Epoll::Poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    ErrorIf(nfds == -1, "epoll wait error");
    std::vector<Channel*> res(nfds);
    for (int i = 0; i != nfds; ++i) {
        Channel* ch = static_cast<Channel*>(events_[i].data.ptr);
        ch->SetReadyEvents(events_[i].events);
        res[i] = ch;
    }
    return res;
}
