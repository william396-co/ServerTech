#include "EventLoop.h"

#include <vector>

#include "Channel.h"
#include "Epoll.h"

EventLoop::EventLoop() { ep_ = new Epoll(); }

EventLoop::~EventLoop() { delete ep_; }

void EventLoop::Loop() {
    while (!quit_) {
        std::vector<Channel*> chs = ep_->Poll();
        for (auto& it : chs) {
            it->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel* ch) { ep_->UpdateChannel(ch); }
