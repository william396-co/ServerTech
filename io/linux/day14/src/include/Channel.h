#pragma once

#include <functional>
#include "Macros.h"
#include "Types.h"

class EventLoop;
class Socket;
class Channel
{
public:
    Channel( EventLoop * loop, Socket * socket );
    ~Channel() {}

    DISALLOW_COPY_AND_MOVE( Channel );

    Socket * getSocket() { return socket_; }
    int getListenEvents() { return listen_events_; }
    int getReadyEvents() { return ready_events_; }
    void getExist() const { return exist_; }
    void setExist( bool in = true ) { exist_ = in; }
    void useET();

    void handleEvent();
    void enableRead();
    void enableWrite();

    void setReadyEvents( int ev );
    void setReadCallback( ChannelReadCallback const & cb ) { read_callback_ = cb; }
    void setWriteCallback( ChannelWriteCallback const & cb ) { write_callback_ = cb; }

private:
    EventLoop * loop_;
    Socket * socket_;
    int listen_events_ { 0 };
    int ready_events_ { 0 };
    bool exist_ { false };
    ChannelReadCallback read_callback_;
    ChannelWriteCallback write_callback_;
};
