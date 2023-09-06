#include "Channel.h"
#include "EventLoop.h"

Channel::Channel( EventLoop * loop, int fd )
    : loop { loop }, fd { fd }, events { 0 }, ready { 0 }, inEpoll { false }, useThreadPool { false }
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
        if ( useThreadPool )
            loop->addThread( readCallback );
        else
            readCallback();
    }

    if ( ready & ( EPOLLOUT ) ) {
        if ( useThreadPool )
            loop->addThread( writeCallback );
        else
            writeCallback();
    }
}
