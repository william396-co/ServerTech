#include "EventLoop.h"

#include <vector>

#include "Channel.h"
#include "Poller.h"

EventLoop::EventLoop() { poller_ = new Poller(); }

EventLoop::~EventLoop() {
    Quit();
    delete poller_;
}

void EventLoop::Loop() {
    while (!quit_) {
        std::vector<Channel*> chs = poller_->Poll();
        for (auto& it : chs) {
            it->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel* ch) { poller_->UpdateChannel(ch); }
void EventLoop::DeleteChannel(Channel* ch) { poller_->DeleteChannel(ch); }
