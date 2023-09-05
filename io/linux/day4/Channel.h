#pragma once

#include <sys/epoll.h>

class Epoll;

class Channel
{
public:
    int getFd() const { return fd; }
    bool getInEpoll() const { return inEpoll; }
    void setInEpoll() { inEpoll = true; }

    Channel( Epoll * ep, int fd );
    ~Channel() {}

    void enableReading();
    uint32_t getEvents() const { return events; }
    uint32_t getRevents() const { return revents; }
    void setRevents( uint32_t ev ) { revents = ev; }

private:
    Epoll * ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
};
