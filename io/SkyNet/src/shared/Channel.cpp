#include "Channel.h"

#include <sys/epoll.h>

#include "EventLoop.h"

Channel::Channel(EventLoop* loop, int fd) : loop_{loop}, fd_{fd} {}

void Channel::HandleEvent() {
    if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
        readCallback_();
    }

    if (ready_events_ & EPOLLOUT) {
        writeCallback_();
    }
}

void Channel::EnableReading() {
    listen_events_ = EPOLLIN | EPOLLPRI;
    loop_->UpdateChannel(this);
}

void Channel::UseET() {
    listen_events_ |= EPOLLET;
    loop_->UpdateChannel(this);
}
