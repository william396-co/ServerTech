#include "Channel.h"
#include "EventLoop.h"

Channel::Channel( EventLoop * loop, int fd )
    : loop { loop }, fd { fd }, events { 0 }, revents { 0 }, inEpoll { false }
{
}

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    loop->updateChannel( this );
}

void Channel::handleEvent()
{
    callback();
}
