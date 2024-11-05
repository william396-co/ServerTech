#pragma once

#include <cstdint>
#include <functional>

class EventLoop;
class Channel
{
    using callbackFunc = std::function<void()>;

public:
    Channel( EventLoop * loop, int fd );
    ~Channel() {}

    void handleEvent();
    void enableReading();
    int getFd() const { return fd_; }

    uint32_t getEvents() const { return events_; }
    void setRevents( uint32_t events ) { revents_ = events; }
    void setCallback( callbackFunc callback ) { callback_ = callback; }
    uint32_t getRevents() const { return revents_; }

    bool isInEpoll() const { return inEpoll; }
    void setInEpoll() { inEpoll = true; }

private:
    EventLoop * loop_ {};
    int fd_ {};
    uint32_t events_ {};
    uint32_t revents_ {};
    bool inEpoll {};
    callbackFunc callback_ {};
};
