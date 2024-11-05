#include "Channel.h"
#include "EventLoop.h"

#include <sys/epoll.h>

Channel::Channel( EventLoop * loop, int fd )
    : loop_ { loop }, fd_ { fd }
{
}

void Channel::handleEvent()
{
    if ( callback_ ) {
        callback_();
    }
}

void Channel::enableReading()
{
    events_ = EPOLLIN | EPOLLET;
    loop_->updateChannel( this );
}

