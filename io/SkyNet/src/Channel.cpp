#include "Channel.h"

#include <sys/epoll.h>

#include "EventLoop.h"

Channel::Channel(EventLoop* loop, int fd) : loop_{loop}, fd_{fd} {}

void Channel::handleEvent() {
    if (ready_ & (EPOLLIN | EPOLLPRI)) {
        readCallback_();
    }

    if (ready_ & EPOLLOUT) {
        writeCallback_();
    }
}

void Channel::enableReading() {
    events_ = EPOLLIN | EPOLLPRI;
    loop_->updateChannel(this);
}

void Channel::useET() {
    events_ |= EPOLLET;
    loop_->updateChannel(this);
}
