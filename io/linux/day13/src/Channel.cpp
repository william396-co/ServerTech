#include "Channel.h"
#include "EventLoop.h"

Channel::Channel( EventLoop * loop, int fd )
    : loop { loop }, fd { fd }, events { 0 }, ready { 0 }, inEpoll { false }
{
}

void Channel::enableRead()
{
    events = EPOLLIN | EPOLLPRI;
    loop->updateChannel( this );
}

void Channel::useET()
{
    events |= EPOLLET;
    loop->updateChannel( this );
}

void Channel::handleEvent()
{
    if ( ready & ( EPOLLIN | EPOLLPRI ) ) {
        readCallback();
    }

    if ( ready & ( EPOLLOUT ) ) {
        writeCallback();
    }
}
