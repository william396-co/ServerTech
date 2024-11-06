#include "Channel.h"
#include "EventLoop.h"

#include <sys/epoll.h>

Channel::Channel( EventLoop * loop, int fd )
    : loop_ { loop }, fd_ { fd }
{
}

void Channel::handleEvent()
{
    if ( ready_ & ( EPOLLIN | EPOLLPRI ) ) {
        if ( useThreadPool_ )
            loop_->addThread( readCallback_ );
        else
            readCallback_();
    }

    if ( ready_ & EPOLLOUT ) {
        if ( useThreadPool_ )
            loop_->addThread( writeCallback_ );
        else
            writeCallback_();
    }
}

void Channel::enableReading()
{
    events_ = EPOLLIN | EPOLLPRI;
    loop_->updateChannel( this );
}

void Channel::useET()
{
    events_ |= EPOLLET;
    loop_->updateChannel( this );
}
