#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Channel::Channel( EventLoop * loop, Socket * socket )
    : loop_ { loop }, socket_ { socket }
{
}

Channel::~Channel()
{
    loop_->deleteChannel( this );
}

Socket * Channel::getSocket()
{
    return socket_;
}

void Channel::enableWrite()
{
    listen_events_ |= WRITE_EVENT;
    loop_->updateChannel( this );
}
void Channel::enableRead()
{
    listen_events_ |= READ_EVENT;
    loop_->updateChannel( this );
}

void Channel::useET()
{
    listen_events_ |= ET;
    loop_->updateChannel( this );
}

void Channel::handleEvent()
{
    if ( ready_events_ & READ_EVENT ) {
        read_callback_();
    }

    if ( ready_events_ & WRITE_EVENT ) {
        write_callback_();
    }
}

void Channel::setReadyEvents( int ev )
{
    if ( ev & READ_EVENT ) {
        ready_events_ |= READ_EVENT;
    }
    if ( ev & WRITE_EVENT ) {
        ready_events_ |= WRITE_EVENT;
    }
    if ( ev & ET ) {
        ready_events_ |= ET;
    }
}
