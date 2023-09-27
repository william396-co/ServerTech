#include "EventLoop.h"

#include "Poller.h"
#include "Channel.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop()
    : poller_ { nullptr }, quit { false }
{
    poller_ = new Poller();
}

EventLoop::~EventLoop()
{
    quit();
    delete poller_;
}

void EventLoop::loop()
{
    while ( !quit ) {
        std::vector<Channel *> chs;
        chs = poller_->poll();
        for ( auto & it : chs ) {
            ( *it ).handleEvent();
        }
    }
}

void EventLoop::updateChannel( Channel * ch )
{
    poller_->updateChannel( ch );
}

void EventLoop::deleteChannel( Channel * ch )
{
    poller_->deleteChannel( ch );
}

void EventLoop::quit()
{
    quit_ = true;
}
