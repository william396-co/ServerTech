#pragma once

#include <sys/epoll.h>
#include <functional>
#include "util.h"

class EventLoop;

class Channel
{
public:
    int getFd() const { return fd; }
    bool getInEpoll() const { return inEpoll; }
    void setInEpoll( bool _in = true ) { inEpoll = _in; }
    void useET();

    Channel( EventLoop * loop, int fd );
    ~Channel() {}

    void handleEvent();
    void enableRead();

    void setReady( uint32_t _ev ) { ready = _ev; }
    uint32_t getReady() const { return ready; }
    uint32_t getEvents() const { return events; }
    void setReadCallback( ChannelReadCallback cb ) { readCallback = cb; }

private:
    EventLoop * loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    ChannelReadCallback readCallback;
    ChannelWriteCallback writeCallback;
};
