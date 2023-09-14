#include "EventLoop.h"

#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop()
    : ep { nullptr }, quit { false }
{
    ep = new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep;
}

void EventLoop::loop()
{
    while ( !quit ) {
        std::vector<Channel *> chs;
        chs = ep->poll();
        for ( auto & it : chs ) {
            ( *it ).handleEvent();
        }
    }
}

void EventLoop::updateChannel( Channel * ch )
{
    ep->updateChannel( ch );
}
