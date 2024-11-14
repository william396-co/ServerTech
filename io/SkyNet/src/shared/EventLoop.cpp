#include "EventLoop.h"

#include <vector>

#include "Channel.h"

EventLoop::EventLoop() { poller_ = std::make_unique<Poller>(); }

EventLoop::~EventLoop() {}

void EventLoop::Loop() const {
    while (true) {
        for (auto& it : poller_->Poll()) {
            it->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel* ch) const { poller_->UpdateChannel(ch); }
void EventLoop::DeleteChannel(Channel* ch) const { poller_->DeleteChannel(ch); }
