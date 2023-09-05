#include "Channel.h"
#include "Epoll.h"

Channel::Channel( Epoll * ep, int fd )
    : ep { ep }, fd { fd }, events { 0 }, revents { 0 }, inEpoll { false }
{
}

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    ep->updateChannel( this );
}
