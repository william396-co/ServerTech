#include "Channel.h"
#include "EventLoop.h"

constexpr auto READ_EVENT = 1;
constexpr auto WRITE_EVENT = 2;
constexpr auto ET = 4;

Channel::Channel( EventLoop * loop, Socket * socket )
    : loop_ { loop }, socket_ { socket }
{
}

Channel::~Channel()
{
    loop_->deleteChannel( this );
}

void Channel::enableWrite()
{
    listen_events_ |= WRITE_EVENT;
    loop_->updateChannel( this );
}
void Channel::enableRead()
{
    listen_events_ |= READ_EVENT;
    loop->updateChannel( this );
}

void Channel::useET()
{
    listen_events_ |= ET;
    loop_->updateChannel( this );
}

void Channel::handleEvent()
{
    if ( ready_events_ & READ_EVENT ) {
        readCallback();
    }

    if ( ready_events_ & WRITE_EVENT ) {
        writeCallback();
    }
}

void Channel::setReadyEvent( int ev )
{
    if ( ev & READ_EVENT ) {
        ready_events_ |= READ_EVENT;
    }
    if ( ev & WRITE_EVENT ) {
        ready_event_ |= WRITE_EVENT;
    }
    if ( ev & ET ) {
        ready_events_ |= ET;
    }
}
