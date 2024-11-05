#pragma once

#include <cstdint>

class Epoll;
class Channel
{
public:
    Channel( Epoll * ep, int fd );
    ~Channel() {}

    void enableReading();
    int getFd() const { return fd_; }

    uint32_t getEvents() const { return events_; }
    void setRevents( uint32_t events ) { revents_ = events; }
    uint32_t getRevents() const { return revents_; }

    bool isInEpoll() const { return inEpoll; }
    void setInEpoll() { inEpoll = true; }

private:
    Epoll * ep_ {};
    int fd_ {};
    uint32_t events_ {};
    uint32_t revents_ {};
    bool inEpoll {};
};
