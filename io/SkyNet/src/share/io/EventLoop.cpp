#include "EventLoop.h"

#include "Epoll.h"
#include "Channel.h"

#include <vector>

EventLoop::EventLoop()
{
    ep_ = new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep_;
}

void EventLoop::loop()
{
    while ( !quit ) {
        std::vector<Channel *> chs = ep_->poll();
        for ( auto & it : chs ) {
            it->handleEvent();
        }
    }
}

void EventLoop::updateChannel( Channel * ch )
{
    ep_->updateChannel( ch );
}
