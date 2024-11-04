#include "Channel.h"
#include "Epoll.h"

#include <sys/epoll.h>

Channel::Channel( Epoll * ep, int fd )
    : ep_ { ep }, fd_ { fd }
{
}

void Channel::enableReading()
{
    events_ = EPOLLIN | EPOLLET;
    ep_->updateChannel( this );
}

