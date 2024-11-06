#pragma once

#include <cstdint>
#include <functional>

class EventLoop;
class Channel
{
    using Callback = std::function<void()>;

public:
    Channel( EventLoop * loop, int fd );
    ~Channel() {}

    void handleEvent();
    void enableReading();
    int getFd() const { return fd_; }

    uint32_t getEvents() const { return events_; }
    void setReady( uint32_t events ) { ready_ = events; }
    uint32_t getReady() const { return ready_; }

    bool isInEpoll() const { return inEpoll_; }
    void setInEpoll( bool in = true ) { inEpoll_ = in; }

    void useET();

    void setReadCallback( Callback cb ) { readCallback_ = cb; }
    void setWriteCallback( Callback cb ) { readCallback_ = cb; }

private:
    EventLoop * loop_ {};
    int fd_ {};
    uint32_t events_ {};
    uint32_t ready_ {};
    bool inEpoll_ {};
    Callback readCallback_ {};
    Callback writeCallback_ {};
};
