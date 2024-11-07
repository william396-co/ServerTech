#include "EventLoop.h"

#include <vector>

#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"

EventLoop::EventLoop() {
    ep_ = new Epoll();
    pool_ = new ThreadPool();
}

EventLoop::~EventLoop() {
    delete ep_;
    delete pool_;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs = ep_->poll();
        for (auto& it : chs) {
            it->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch) { ep_->updateChannel(ch); }

void EventLoop::addThread(Task task) { pool_->add(task); }
