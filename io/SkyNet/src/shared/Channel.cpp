#include "Channel.h"

#include "EventLoop.h"
#include "Socket.h"

Channel::Channel(EventLoop* loop, Socket* s) : loop_{loop}, s_{s} {}

Channel::~Channel() { loop_->DeleteChannel(this); }

void Channel::HandleEvent() {
    if (ready_events_ & READ_EVENT) {
        read_callback_();
    }

    if (ready_events_ & WRITE_EVENT) {
        write_callback_();
    }
}

void Channel::EnableWrite() {
    listen_events_ |= WRITE_EVENT;
    loop_->UpdateChannel(this);
}

void Channel::EnableRead() {
    listen_events_ = READ_EVENT;
    loop_->UpdateChannel(this);
}

void Channel::UseET() {
    listen_events_ |= ET;
    loop_->UpdateChannel(this);
}

void Channel::SetReadyEvents(int ev) {
    if (ev & READ_EVENT) {
        ready_events_ |= READ_EVENT;
    }
    if (ev & WRITE_EVENT) {
        ready_events_ |= WRITE_EVENT;
    }
    if (ev & ET) {
        ready_events_ |= ET;
    }
}
