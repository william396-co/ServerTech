#pragma once

#include <sys/epoll.h>
#include <functional>

class EventLoop;
using ChannelCallback = std::function<void()>;

class Channel
{
public:
    int getFd() const { return fd; }
    bool getInEpoll() const { return inEpoll; }
    void setInEpoll() { inEpoll = true; }

    Channel( EventLoop * loop, int fd );
    ~Channel() {}

    void handleEvent();
    void enableReading();
    uint32_t getEvents() const { return events; }
    uint32_t getRevents() const { return revents; }
    void setRevents( uint32_t ev ) { revents = ev; }
    void setCallback( ChannelCallback cb ) { callback = cb; }

private:
    EventLoop * loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    ChannelCallback callback = nullptr;
};
