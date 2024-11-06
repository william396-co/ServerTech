#include "EventLoop.h"

#include "Epoll.h"
#include "Channel.h"
#include "utils/ThreadPool.h"

#include <vector>

EventLoop::EventLoop()
{
    ep_ = new Epoll();
    pool_ = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete ep_;
    delete pool_;
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

void EventLoop::addThread( Task task )
{
    pool_->add( task );
}
